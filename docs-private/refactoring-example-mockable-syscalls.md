# Refactoring Example: Mockable System Calls

This document shows how to refactor pipex code to use function pointers for mockable system calls - the pattern you'll need for minishell.

---

## 🎯 Goal

Transform this code from **untestable** to **fully testable with mocks**:

```c
// Before: Hard-coded system calls (untestable)
int fd = open(filename, O_RDONLY);
pid_t pid = fork();
```

```c
// After: Mockable system calls (testable)
int fd = g_syscalls->open(filename, O_RDONLY);
pid_t pid = g_syscalls->fork();
```

---

## 📁 Step 1: Create System Call Wrapper Interface

```c
// include/syscall_interface.h
#ifndef SYSCALL_INTERFACE_H
# define SYSCALL_INTERFACE_H

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>

/**
 * System call interface for dependency injection
 * Allows mocking in tests, real syscalls in production
 */
typedef struct s_syscall_ops
{
	// File operations
	int		(*open)(const char *pathname, int flags, ...);
	ssize_t	(*read)(int fd, void *buf, size_t count);
	ssize_t	(*write)(int fd, const void *buf, size_t count);
	int		(*close)(int fd);
	int		(*access)(const char *pathname, int mode);
	
	// Process operations
	pid_t	(*fork)(void);
	int		(*execve)(const char *path, char *const argv[], char *const envp[]);
	pid_t	(*waitpid)(pid_t pid, int *status, int options);
	
	// IPC operations
	int		(*pipe)(int pipefd[2]);
	int		(*dup2)(int oldfd, int newfd);
	
	// Signal operations (for minishell)
	int		(*kill)(pid_t pid, int sig);
	// void	(*signal)(int signum, void (*handler)(int));
	
}	t_syscall_ops;

// Global syscall operations (points to real or mock)
extern t_syscall_ops	*g_syscalls;

// Initialize with real system calls (production)
void	syscalls_use_real(void);

// Initialize with custom ops (testing)
void	syscalls_use_custom(t_syscall_ops *ops);

#endif
```

---

## 📝 Step 2: Implement Real System Call Wrappers

```c
// src/syscall_interface.c
#include "syscall_interface.h"
#include <stdlib.h>
#include <stdarg.h>

// Real system call implementations
static int	real_open(const char *pathname, int flags, ...)
{
	if (flags & O_CREAT)
	{
		va_list	args;
		mode_t	mode;

		va_start(args, flags);
		mode = va_arg(args, mode_t);
		va_end(args);
		return (open(pathname, flags, mode));
	}
	return (open(pathname, flags));
}

static ssize_t	real_read(int fd, void *buf, size_t count)
{
	return (read(fd, buf, count));
}

static ssize_t	real_write(int fd, const void *buf, size_t count)
{
	return (write(fd, buf, count));
}

static int	real_close(int fd)
{
	return (close(fd));
}

static int	real_access(const char *pathname, int mode)
{
	return (access(pathname, mode));
}

static pid_t	real_fork(void)
{
	return (fork());
}

static int	real_execve(const char *path, char *const argv[], char *const envp[])
{
	return (execve(path, argv, envp));
}

static pid_t	real_waitpid(pid_t pid, int *status, int options)
{
	return (waitpid(pid, status, options));
}

static int	real_pipe(int pipefd[2])
{
	return (pipe(pipefd));
}

static int	real_dup2(int oldfd, int newfd)
{
	return (dup2(oldfd, newfd));
}

static int	real_kill(pid_t pid, int sig)
{
	return (kill(pid, sig));
}

// Default operations (real system calls)
static t_syscall_ops	real_ops = {
	.open = real_open,
	.read = real_read,
	.write = real_write,
	.close = real_close,
	.access = real_access,
	.fork = real_fork,
	.execve = real_execve,
	.waitpid = real_waitpid,
	.pipe = real_pipe,
	.dup2 = real_dup2,
	.kill = real_kill,
};

// Global pointer (defaults to real operations)
t_syscall_ops	*g_syscalls = &real_ops;

void	syscalls_use_real(void)
{
	g_syscalls = &real_ops;
}

void	syscalls_use_custom(t_syscall_ops *ops)
{
	g_syscalls = ops;
}
```

---

## 🔧 Step 3: Refactor Existing Code to Use Interface

### Before: Direct System Calls

```c
// src/child1/child1_cmd1.c (BEFORE)
static void	open_infile1(char *filename, int *file1)
{
	*file1 = open(filename, O_RDONLY);  // ← Hard-coded
	if (*file1 == -1)
		fatal_sys(filename, 1);
}

void	child1_cmd1(char *file1, int p[2], char *argv, char *const envp[])
{
	int	fd1;

	open_infile1(file1, &fd1);
	xclose(p[0]);
	if (dup2(fd1, 0) == -1)  // ← Hard-coded
		fatal_sys("dup2 failed", 1);
	xclose(fd1);
	if (dup2(p[1], 1) == -1)  // ← Hard-coded
		fatal_sys("dup2 failed", 1);
	xclose(p[1]);
	callexecve1(argv, envp);
}
```

### After: Using Function Pointers

```c
// src/child1/child1_cmd1.c (AFTER)
#include "syscall_interface.h"  // ← Add this

static void	open_infile1(char *filename, int *file1)
{
	*file1 = g_syscalls->open(filename, O_RDONLY);  // ← Use interface
	if (*file1 == -1)
		fatal_sys(filename, 1);
}

void	child1_cmd1(char *file1, int p[2], char *argv, char *const envp[])
{
	int	fd1;

	open_infile1(file1, &fd1);
	xclose(p[0]);
	if (g_syscalls->dup2(fd1, 0) == -1)  // ← Use interface
		fatal_sys("dup2 failed", 1);
	xclose(fd1);
	if (g_syscalls->dup2(p[1], 1) == -1)  // ← Use interface
		fatal_sys("dup2 failed", 1);
	xclose(p[1]);
	callexecve1(argv, envp);
}
```

### Refactor error.c Wrappers

```c
// src/utils/error.c (AFTER)
#include "syscall_interface.h"

void	xclose(int fd)
{
	if (g_syscalls->close(fd) == -1)  // ← Use interface
		fatal_sys("close", 1);
}

pid_t	xfork(void)
{
	pid_t	pid;

	pid = g_syscalls->fork();  // ← Use interface
	if (pid == -1)
		fatal_sys("fork", 1);
	return (pid);
}
```

### Refactor main.c

```c
// src/main.c (AFTER)
#include "syscall_interface.h"

int	main(int argc, char **argv, char *const envp[])
{
	int		p[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status_cmd2;

	// Initialize real syscalls (production)
	syscalls_use_real();  // ← Add this

	if (argc != 5)
		error_fd2("usage", "./pipex file1 cmd1 cmd2 file2", EXIT_FAILURE);
	
	if (g_syscalls->pipe(p) == -1)  // ← Use interface
		exit(-1);
	
	pid1 = xfork();
	if (pid1 == 0)
		child1_cmd1(argv[1], p, argv[2], envp);
	
	pid2 = xfork();
	if (pid2 == 0)
		child2_cmd2(argv[4], p, argv[3], envp);
	
	xclose(p[0]);
	xclose(p[1]);
	
	g_syscalls->waitpid(pid2, &status_cmd2, 0);  // ← Use interface
	g_syscalls->waitpid(pid1, NULL, 0);  // ← Use interface
	
	if (WIFEXITED(status_cmd2))
		exit(WEXITSTATUS(status_cmd2));
	else
		exit(1);
}
```

### Refactor path_from_cmdname.c

```c
// src/utils/path_from_cmdname.c (AFTER)
#include "syscall_interface.h"

char	*path_from_cmdname(char *arg, char *const envp[])
{
	char	**paths;
	char	*path;
	char	*bar;
	char	**paths_start;

	envp = get_path_envp(envp);
	if (!(*envp))
		return (NULL);
	paths = ft_split(*envp + 5, ':');
	paths_start = paths;
	while (paths && *paths)
	{
		bar = ft_strjoin(*paths, "/");
		path = ft_strjoin(bar, arg);
		free(bar);
		
		// ← Use interface instead of access()
		if (g_syscalls->access(path, F_OK) == 0)
			return (ft_split_free(paths_start), path);
		
		free(path);
		paths++;
	}
	ft_split_free(paths_start);
	return (NULL);
}
```

---

## ✅ Step 4: Create Mock System Calls for Tests

```c
// tests/unit/mocks/mock_syscalls.c
#include <criterion/criterion.h>
#include <errno.h>
#include <string.h>
#include "../../../include/syscall_interface.h"

// Mock state for tracking calls
typedef struct s_mock_state {
	int		open_call_count;
	int		fork_call_count;
	int		should_fail_open;
	int		should_fail_fork;
	char	*last_opened_file;
}	t_mock_state;

static t_mock_state	g_mock_state = {0};

// Mock open() that can be controlled
static int	mock_open(const char *pathname, int flags, ...)
{
	g_mock_state.open_call_count++;
	
	// Save last opened file for verification
	if (g_mock_state.last_opened_file)
		free(g_mock_state.last_opened_file);
	g_mock_state.last_opened_file = strdup(pathname);
	
	// Simulate failure if flag is set
	if (g_mock_state.should_fail_open)
	{
		errno = EACCES;
		return (-1);
	}
	
	// Simulate specific file failures
	if (strstr(pathname, "no-perms"))
	{
		errno = EACCES;
		return (-1);
	}
	
	// Return fake fd (> 2 to avoid stdout/stderr)
	return (42);
}

// Mock fork() that can be controlled
static pid_t	mock_fork(void)
{
	g_mock_state.fork_call_count++;
	
	// Simulate fork failure if flag is set
	if (g_mock_state.should_fail_fork)
	{
		errno = EAGAIN;
		return (-1);
	}
	
	// Return fake pid (for testing parent path)
	return (1000);
}

// Mock close() (always succeeds in tests)
static int	mock_close(int fd)
{
	(void)fd;
	return (0);
}

// Mock pipe() that can be controlled
static int	mock_pipe(int pipefd[2])
{
	pipefd[0] = 10;  // Fake read end
	pipefd[1] = 11;  // Fake write end
	return (0);
}

// Mock dup2() (always succeeds in tests)
static int	mock_dup2(int oldfd, int newfd)
{
	(void)oldfd;
	(void)newfd;
	return (newfd);
}

// Mock access() that can be controlled
static int	mock_access(const char *pathname, int mode)
{
	(void)mode;
	
	// Simulate common commands exist
	if (strstr(pathname, "/usr/bin/ls") ||
		strstr(pathname, "/usr/bin/cat") ||
		strstr(pathname, "/usr/bin/grep"))
		return (0);
	
	// Everything else doesn't exist
	errno = ENOENT;
	return (-1);
}

// Mock waitpid() (returns immediately)
static pid_t	mock_waitpid(pid_t pid, int *status, int options)
{
	(void)options;
	
	if (status)
		*status = 0;  // Exited with status 0
	
	return (pid);
}

// Mock execve() (doesn't actually exec)
static int	mock_execve(const char *path, char *const argv[], char *const envp[])
{
	(void)path;
	(void)argv;
	(void)envp;
	
	// In real code, execve never returns on success
	// In mock, we return to allow test to continue
	return (0);
}

// Mock operations struct
static t_syscall_ops	mock_ops = {
	.open = mock_open,
	.close = mock_close,
	.fork = mock_fork,
	.pipe = mock_pipe,
	.dup2 = mock_dup2,
	.access = mock_access,
	.waitpid = mock_waitpid,
	.execve = mock_execve,
};

// Helper functions to control mocks
void	mock_reset(void)
{
	g_mock_state.open_call_count = 0;
	g_mock_state.fork_call_count = 0;
	g_mock_state.should_fail_open = 0;
	g_mock_state.should_fail_fork = 0;
	
	if (g_mock_state.last_opened_file)
	{
		free(g_mock_state.last_opened_file);
		g_mock_state.last_opened_file = NULL;
	}
}

void	mock_set_open_failure(int should_fail)
{
	g_mock_state.should_fail_open = should_fail;
}

void	mock_set_fork_failure(int should_fail)
{
	g_mock_state.should_fail_fork = should_fail;
}

int	mock_get_open_call_count(void)
{
	return (g_mock_state.open_call_count);
}

int	mock_get_fork_call_count(void)
{
	return (g_mock_state.fork_call_count);
}

char	*mock_get_last_opened_file(void)
{
	return (g_mock_state.last_opened_file);
}

void	mock_syscalls_enable(void)
{
	mock_reset();
	syscalls_use_custom(&mock_ops);
}

void	mock_syscalls_disable(void)
{
	mock_reset();
	syscalls_use_real();
}
```

---

## 🧪 Step 5: Write Tests Using Mocks

```c
// tests/unit/test_with_mocks.c
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../include/pipex.h"
#include "../../include/syscall_interface.h"
#include "mocks/mock_syscalls.h"

// Setup and teardown
void	setup_mocks(void)
{
	mock_syscalls_enable();
}

void	teardown_mocks(void)
{
	mock_syscalls_disable();
}

TestSuite(pipex_with_mocks, .init = setup_mocks, .fini = teardown_mocks);

// Test 1: open() is called with correct filename
Test(pipex_with_mocks, open_called_with_correct_filename)
{
	// This would normally require refactoring to not call exit()
	// For demonstration, let's test path_from_cmdname with mock access
	
	char *envp[] = {"PATH=/usr/bin:/bin", NULL};
	char *result = path_from_cmdname("ls", envp);
	
	cr_assert_not_null(result);
	cr_assert_str_eq(result, "/usr/bin/ls");
	
	free(result);
}

// Test 2: handles open() failure gracefully
Test(pipex_with_mocks, handles_open_failure, .exit_code = 1)
{
	// Enable open() failure
	mock_set_open_failure(1);
	
	// This test would work after refactoring open_infile1
	// to not call exit(), or using .exit_code expectation
	
	// For now, this demonstrates the pattern
	int fd;
	fd = g_syscalls->open("test.txt", O_RDONLY);
	
	cr_assert_eq(fd, -1);
	cr_assert_eq(errno, EACCES);
}

// Test 3: handles fork() failure
Test(pipex_with_mocks, handles_fork_failure)
{
	mock_set_fork_failure(1);
	
	pid_t pid = g_syscalls->fork();
	
	cr_assert_eq(pid, -1);
	cr_assert_eq(errno, EAGAIN);
}

// Test 4: pipe() is called and returns valid fds
Test(pipex_with_mocks, pipe_returns_valid_fds)
{
	int p[2];
	int result = g_syscalls->pipe(p);
	
	cr_assert_eq(result, 0);
	cr_assert_eq(p[0], 10);  // Mock read end
	cr_assert_eq(p[1], 11);  // Mock write end
}

// Test 5: Test command execution without actually execing
Test(pipex_with_mocks, command_resolution_works)
{
	char *envp[] = {"PATH=/usr/bin:/bin", NULL};
	
	// Test that path resolution works with mocked access()
	char *result = path_from_cmdname("grep", envp);
	
	cr_assert_not_null(result);
	cr_assert(strstr(result, "grep") != NULL);
	
	free(result);
}

// Test 6: Permission denied scenario (works in Docker!)
Test(pipex_with_mocks, permission_denied_portable, .init = cr_redirect_stderr)
{
	// Mock will always fail for files with "no-perms"
	int fd = g_syscalls->open("infile-no-perms", O_RDONLY);
	
	cr_assert_eq(fd, -1);
	cr_assert_eq(errno, EACCES);
	
	// This test works whether you're root or not!
}
```

---

## 📊 Step 6: Update Makefile

```makefile
# Add syscall_interface to SRCS
SRCS_PROD := src/main.c \
    src/child1/child1_cmd1.c \
    src/child2/child2_cmd2.c \
    src/utils/path_from_cmdname.c \
    src/utils/error.c \
    src/syscall_interface.c

# Add mock sources for unit tests
MOCK_SRCS := tests/unit/mocks/mock_syscalls.c
MOCK_OBJS := $(MOCK_SRCS:.c=.o)

# Unit tests now include mocks
$(UNIT_BIN): $(UNIT_OBJS) $(MOCK_OBJS) $(OBJS_CORE) $(LIBFT_LIB)
	$(Q)$(CC) $(CFLAGS) $(CRIT_CFLAGS) \
		$(UNIT_OBJS) $(MOCK_OBJS) $(OBJS_CORE) $(LIBFT_LIB) $(CRIT_LIBS) \
		-o $@
	./$@ --color --fail-fast
```

---

## 🎯 Benefits of This Refactoring

### Before (Hard-Coded System Calls)
- ❌ Can't test fork failure scenarios
- ❌ Can't test file permission issues in Docker
- ❌ Can't test resource exhaustion
- ❌ Tests fail in different environments
- ❌ Hard to test error handling

### After (Mockable System Calls)
- ✅ Can test all failure scenarios
- ✅ Tests work in Docker as root
- ✅ Can simulate resource exhaustion
- ✅ Tests are portable across environments
- ✅ Easy to test error handling
- ✅ Ready for TDD in minishell

---

## 🚀 Using This Pattern in Minishell

For minishell, you'll want to mock even more:

```c
// For minishell: Extended syscall interface
typedef struct s_minishell_syscalls
{
	// All the pipex ones, plus:
	int		(*chdir)(const char *path);
	char	*(*getcwd)(char *buf, size_t size);
	char	*(*getenv)(const char *name);
	int		(*setenv)(const char *name, const char *value, int overwrite);
	int		(*unsetenv)(const char *name);
	int		(*isatty)(int fd);
	void	(*signal)(int signum, sighandler_t handler);
	// ... more as needed
}	t_minishell_syscalls;
```

Then test builtins without side effects:

```c
Test(builtins, cd_changes_directory_mock)
{
	// Mock getcwd to return fake current dir
	// Mock chdir to track calls
	// Test cd builtin logic without actually changing directory
}
```

---

## 📝 Summary

**What we did**:
1. Created syscall interface with function pointers
2. Implemented real syscall wrappers
3. Refactored code to use `g_syscalls->` instead of direct calls
4. Created mock syscalls for testing
5. Wrote tests that work in any environment

**Result**:
- Production code uses real system calls
- Test code uses mocks
- Tests are portable (Docker, campus, macOS)
- Ready for TDD approach in minishell

**Effort**:
- Initial setup: ~2-3 hours
- Refactoring existing code: ~1 hour
- Creating mocks: ~1 hour
- **Total**: ~4-5 hours

**Payoff for Minishell**:
- Save weeks of debugging
- Catch edge cases early
- TDD becomes possible
- Tests run anywhere
- **Worth it**: ✅✅✅

