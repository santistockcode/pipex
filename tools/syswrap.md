# Syswrap: Syscall Wrappers and Failure Injection

This document explains how the syscall wrapper layer works, how environment variables drive failures, how the global function pointers can override behavior, and how to interact with the layer from Python via `ctypes`.

## Overview

Wrappers are defined in `include/syswrap.h` and implemented in `src/utils/syswrap.c`:

- `int pipe_wrap(int p[2]);`
- `pid_t fork_wrap(void);`
- `int dup2_wrap(int oldfd, int newfd);`
- `int open_wrap(const char *path, int oflag, ...);`
- `int execve_wrap(const char *path, char *const argv[], char *const envp[]);`

Each wrapper has two possible injection mechanisms:
1) Environment variables: checked on every call; when set, the wrapper fails with a specific `errno`.
2) Function pointer override (global setters): tests can install a custom function to replace the underlying syscall.

Production code calls the wrappers so tests can deterministically simulate failures.

## Environment Variables (No code changes required)

The wrappers inspect environment variables at call time:

- `PIPEX_FAIL_PIPE_AT=N` → fail the Nth `pipe()` with `errno=EMFILE`.
- `PIPEX_FAIL_FORK=1` → fail all `fork()` calls with `errno=EAGAIN`.
- `PIPEX_FAIL_DUP2_AT=N` → fail the Nth `dup2()` with `errno=EBADF`.
- `PIPEX_FAIL_OPEN_PATH=substr` → fail `open()` when the path contains `substr` with `errno=EACCES`.
- `PIPEX_FAIL_EXECVE=1` → fail all `execve()` calls with `errno=EACCES`.

Example:
```
PIPEX_FAIL_PIPE_AT=4 ./pipex input "cat" "grep hello" "wc -l" "cat" "wc -c" outfile
```

Important: Environment-based failures do not require function pointer overrides. In this case, the global pointer (e.g. `g_pipe_fn`) remains `NULL`. The wrapper first checks the env, and only if no env-triggered failure applies does it defer to the function pointer (if set), else it calls the real syscall.

## Function Pointer Overrides (Advanced)

You can install custom functions to intercept calls. The header provides typedefs and setters:

```c
typedef int   (*pipe_fn_t)(int p[2]);
typedef pid_t (*fork_fn_t)(void);
typedef int   (*dup2_fn_t)(int, int);
typedef int   (*open_fn_t)(const char *path, int oflag, ...);
typedef int   (*execve_fn_t)(const char *path, char *const argv[], char *const envp[]);

void syswrap_set_pipe(pipe_fn_t fn);
void syswrap_set_fork(fork_fn_t fn);
void syswrap_set_dup2(dup2_fn_t fn);
void syswrap_set_open(open_fn_t fn);
void syswrap_set_execve(execve_fn_t fn);
```

When a pointer is set, the wrapper calls your function after checking env controls. This lets tests inject custom behavior beyond simple failure (e.g., counting arguments, logging, conditional behavior).

## Control Flow Inside Wrappers

Taking `pipe_wrap` as an example:

1. Increment internal call counter.
2. Check `PIPEX_FAIL_PIPE_AT`. If it matches, set `errno=EMFILE` and return `-1`.
3. If no env failure, check `g_pipe_fn`. If non-NULL, call it.
4. Otherwise, call the real `pipe`.

Other wrappers follow the same pattern with their respective env controls and function pointers.

## Python `ctypes` Usage

You can use `ctypes` to interact with exported setter symbols to install overrides from Python. This requires building a shared object or ensuring the binary exports the setter functions. The simplest approach is to compile the core into a shared library (e.g., `libpipexcore.so`) for tests.

### Example: Overriding `pipe_wrap` via a custom C shim

Write a tiny C shim implementing a failing `pipe` function compatible with `pipe_fn_t`:

```c
// fail_pipe.c
#include <errno.h>
int fail_pipe_once(int p[2]) { errno = EMFILE; return -1; }
```

Compile it as a shared object:
```
cc -fPIC -shared -o libfailpipe.so fail_pipe.c
```

From Python:
```python
import ctypes

# Load the main binary or shared core where syswrap_set_pipe is exported
core = ctypes.CDLL('./pipex')  # or './libpipexcore.so'

# Load your fail implementation
fail = ctypes.CDLL('./libfailpipe.so')

# Declare signatures
core.syswrap_set_pipe.argtypes = [ctypes.CFUNCTYPE(ctypes.c_int, ctypes.POINTER(ctypes.c_int))]
core.syswrap_set_pipe.restype  = None

fail_func_type = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.POINTER(ctypes.c_int))
fail_func = fail_func_type(('fail_pipe_once', fail))

# Install the override
core.syswrap_set_pipe(fail_func)

# Now run commands that exercise pipe_wrap (you may need an entry point that calls into the pipeline)
# If calling the standalone binary, setters must be called inside its process; typically, you’d embed
# a test harness or expose an init function. Alternatively, prefer env controls for the standalone binary.
```

Notes:
- For a standalone binary, calling setters from a separate Python process does not affect the already-running binary. Use env variables for the standalone case.
- For in-process tests (embedding Python or a C test harness), setters work well.
- Export visibility: ensure the setters are not `static` and are linked into the testable artifact.

### Using Environment Variables from `ctypes`/Python

You can also drive failures by setting env vars before launching the binary:

```python
import os, subprocess
env = dict(os.environ)
env['PIPEX_FAIL_PIPE_AT'] = '4'
subprocess.run(['./pipex', 'input', 'cat', 'wc', 'outfile'], env=env)
```

This path requires no shared library setup and is generally the simplest for integration tests.

## FAQ

Q: “How does `PIPEX_FAIL_PIPE_AT` trigger if `g_pipe_fn` is NULL?”

A: The wrapper checks the environment first. If the env condition matches, it returns failure immediately and never reaches the function pointer branch. The global pointer stays `NULL` unless you explicitly set it via `syswrap_set_*`. Environment conditions alone are sufficient for failure injection.

Q: “Can I combine env and function pointers?”

A: Yes. Env checks run first; if no failure, the wrapper uses your function pointer if set. This lets you mix deterministic failure points with richer custom behavior.

Q: “Do wrappers affect stderr/stdout?”

A: No. They only affect the specific syscall being wrapped. Your error reporting still uses `fatal_sys` (stderr) and child exit handling. Combine with debug builds (`-DDEBUG`) to trace behavior.

