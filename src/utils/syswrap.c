/* Syscall wrappers with optional failure injection via environment variables.
 * Env controls (read at call time):
 *   PIPEX_FAIL_PIPE_AT=N  -> fail on Nth pipe call (errno=EMFILE)
 *   PIPEX_FAIL_FORK=1     -> always fail fork (errno=EAGAIN)
 *   PIPEX_FAIL_DUP2_AT=N  -> fail on Nth dup2 (errno=EBADF)
 *   PIPEX_FAIL_OPEN_PATH=substring -> fail open if path contains substring (errno=EACCES)
 *   PIPEX_FAIL_EXECVE=1   -> always fail execve (errno=EACCES)
 */

#include "../../include/syswrap.h"
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

static pipe_fn_t   g_pipe_fn   = NULL;
static fork_fn_t   g_fork_fn   = NULL;
static dup2_fn_t   g_dup2_fn   = NULL;
static open_fn_t   g_open_fn   = NULL;
static execve_fn_t g_execve_fn = NULL;

/* Simple counters for _AT style failure controls */
static unsigned long pipe_calls = 0;
static unsigned long dup2_calls = 0;

void syswrap_set_pipe(pipe_fn_t fn)   { g_pipe_fn = fn; }
void syswrap_set_fork(fork_fn_t fn)   { g_fork_fn = fn; }
void syswrap_set_dup2(dup2_fn_t fn)   { g_dup2_fn = fn; }
void syswrap_set_open(open_fn_t fn)   { g_open_fn = fn; }
void syswrap_set_execve(execve_fn_t fn){ g_execve_fn = fn; }

int pipe_wrap(int p[2])
{
    pipe_calls++;
    const char *fail_at = getenv("PIPEX_FAIL_PIPE_AT");
    if (fail_at)
    {
        unsigned long target = strtoul(fail_at, NULL, 10);
        if (target > 0 && pipe_calls == target)
        {
            errno = EMFILE; /* too many fds */
            return -1;
        }
    }
    if (g_pipe_fn)
        return g_pipe_fn(p); /* custom injected */
    return pipe(p);
}

pid_t fork_wrap(void)
{
    const char *fail = getenv("PIPEX_FAIL_FORK");
    if (fail && fail[0] == '1')
    {
        errno = EAGAIN;
        return -1;
    }
    if (g_fork_fn)
        return g_fork_fn();
    return fork();
}

int dup2_wrap(int oldfd, int newfd)
{
    dup2_calls++;
    const char *fail_at = getenv("PIPEX_FAIL_DUP2_AT");
    if (fail_at)
    {
        unsigned long target = strtoul(fail_at, NULL, 10);
        if (target > 0 && dup2_calls == target)
        {
            errno = EBADF;
            return -1;
        }
    }
    if (g_dup2_fn)
        return g_dup2_fn(oldfd, newfd);
    return dup2(oldfd, newfd);
}

int open_wrap(const char *path, int oflag, ...)
{
    const char *fail_sub = getenv("PIPEX_FAIL_OPEN_PATH");
    if (fail_sub && path && strstr(path, fail_sub))
    {
        errno = EACCES;
        return -1;
    }
    int mode = 0;
    if (oflag & O_CREAT)
    {
        va_list ap;
        va_start(ap, oflag);
        mode = va_arg(ap, int);
        va_end(ap);
    }
    if (g_open_fn)
        return g_open_fn(path, oflag, mode);
    if (oflag & O_CREAT)
        return open(path, oflag, mode);
    return open(path, oflag);
}

int execve_wrap(const char *path, char *const argv[], char *const envp[])
{
    const char *fail = getenv("PIPEX_FAIL_EXECVE");
    if (fail && fail[0] == '1')
    {
        errno = EACCES;
        return -1;
    }
    if (g_execve_fn)
        return g_execve_fn(path, argv, envp);
    return execve(path, argv, envp);
}