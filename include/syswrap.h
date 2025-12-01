/* Syscall wrapper layer allowing injection/failure control for integration tests */
#ifndef SYSWRAP_H
#define SYSWRAP_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

/* Wrapper prototypes used across the codebase */
int     pipe_wrap(int p[2]);
pid_t   fork_wrap(void);
int     dup2_wrap(int oldfd, int newfd);
int     open_wrap(const char *path, int oflag, ...);
int     execve_wrap(const char *path, char *const argv[], char *const envp[]);

/* Optional setters to override behavior (tests can inject stubs) */
typedef int   (*pipe_fn_t)(int p[2]);
typedef pid_t (*fork_fn_t)(void);
typedef int   (*dup2_fn_t)(int, int);
typedef int   (*open_fn_t)(const char *path, int oflag, ...);
typedef int   (*execve_fn_t)(const char *path, char *const argv[], char *const envp[]);

void    syswrap_set_pipe(pipe_fn_t fn);
void    syswrap_set_fork(fork_fn_t fn);
void    syswrap_set_dup2(dup2_fn_t fn);
void    syswrap_set_open(open_fn_t fn);
void    syswrap_set_execve(execve_fn_t fn);

#endif /* SYSWRAP_H */