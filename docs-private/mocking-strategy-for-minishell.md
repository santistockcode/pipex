# Mocking System Calls: Strategy for Minishell

**Context**: Preparing pipex testing approach for minishell development  
**Problem**: Docker permission issues, need for controlled testing environments  
**Solution**: Mock system calls for unit tests, keep integration tests for E2E

---

## 🎯 Why Mock System Calls for Minishell?

### The Docker Problem You're Experiencing

```bash
# Campus machine (normal user)
$ chmod 000 infile-no-perms
$ ./pipex infile-no-perms "cat" "wc" outfile
pipex: infile-no-perms: Permission denied  ✅

# Docker (root user)
$ chmod 000 infile-no-perms
$ ./pipex infile-no-perms "cat" "wc" outfile
# Root bypasses permissions ❌
```

### Why This Matters for Minishell

Minishell is **10x more complex** than pipex:
- Multiple pipes (`cmd1 | cmd2 | cmd3 | cmd4`)
- Redirections (`<`, `>`, `>>`, `<<`)
- Signal handling (Ctrl-C, Ctrl-D, Ctrl-\\)
- Builtins (cd, echo, env, export, unset, exit)
- Environment variable expansion
- Quote handling
- Job control (maybe)

**Testing challenges**:
1. **Fork failures** - How to test when system runs out of processes?
2. **Pipe failures** - How to test when FD limit is reached?
3. **Signal handling** - How to test without actually sending signals?
4. **Permission errors** - How to test reliably across environments?
5. **Resource exhaustion** - How to test edge cases?

---

## 🛠️ Mocking Approaches in C

### Option 1: LD_PRELOAD (Recommended for Integration Tests)

**How it works**: Intercept system calls at runtime by loading your mock library first.

#### Example: Mock `open()` to Simulate Permission Denied

```c
// tests/mocks/mock_syscalls.c
#define _GNU_SOURCE
#include <dlfcn.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

// Original function pointer
static int (*real_open)(const char *, int, ...) = NULL;

// Global flag to control mock behavior
static int force_open_failure = 0;

void mock_open_set_failure(int should_fail) {
    force_open_failure = should_fail;
}

// Intercepted open()
int open(const char *pathname, int flags, ...) {
    // Load real open() on first call
    if (!real_open) {
        real_open = dlsym(RTLD_NEXT, "open");
    }

    // Simulate permission denied if flag is set
    if (force_open_failure && strstr(pathname, "infile-no-perms")) {
        errno = EACCES;
        return -1;
    }

    // Call real open()
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode_t mode = va_arg(args, mode_t);
        va_end(args);
        return real_open(pathname, flags, mode);
    }
    return real_open(pathname, flags);
}
```

```bash
# Compile mock library
gcc -shared -fPIC -o libmock.so tests/mocks/mock_syscalls.c -ldl

# Run tests with mock
LD_PRELOAD=./libmock.so ./pipex infile-no-perms "cat" "wc" outfile
```

**Pros**:
- ✅ Works with existing binary
- ✅ No code changes needed
- ✅ Perfect for Docker permission issues

**Cons**:
- ❌ Complex to set up
- ❌ Hard to control from test code
- ❌ Platform-specific (Linux mainly)

---

### Option 2: Function Pointers (Recommended for Unit Tests)

**How it works**: Pass function pointers to your code, swap them in tests.

#### Example: Refactor to Use Function Pointers

```c
// include/syscall_wrappers.h
typedef struct s_syscalls {
    int (*open)(const char *, int, ...);
    ssize_t (*read)(int, void *, size_t);
    ssize_t (*write)(int, const void *, size_t);
    pid_t (*fork)(void);
    int (*pipe)(int[2]);
    int (*execve)(const char *, char *const[], char *const[]);
    // ... more syscalls
} t_syscalls;

// Global syscalls (default to real ones)
extern t_syscalls *g_syscalls;

// Initialize with real syscalls
void syscalls_init_real(void);

// Initialize with mocks (for tests)
void syscalls_init_mock(void);
```

```c
// src/syscall_wrappers.c
#include <unistd.h>
#include <fcntl.h>
#include "syscall_wrappers.h"

static t_syscalls real_syscalls = {
    .open = open,
    .read = read,
    .write = write,
    .fork = fork,
    .pipe = pipe,
    .execve = execve,
};

t_syscalls *g_syscalls = &real_syscalls;

void syscalls_init_real(void) {
    g_syscalls = &real_syscalls;
}
```

```c
// src/child1/child1_cmd1.c (REFACTORED)
static void open_infile1(char *filename, int *file1)
{
    *file1 = g_syscalls->open(filename, O_RDONLY);  // ← Use function pointer
    if (*file1 == -1)
        fatal_sys(filename, 1);
}
```

```c
// tests/unit/test_with_mocks.c
#include <criterion/criterion.h>
#include "../../include/syscall_wrappers.h"

// Mock open() that always fails
int mock_open_fail(const char *path, int flags, ...) {
    errno = EACCES;
    return -1;
}

// Mock fork() that simulates resource exhaustion
pid_t mock_fork_fail(void) {
    errno = EAGAIN;
    return -1;
}

Test(child1_cmd1, handles_open_failure) {
    // Arrange: Set up mock syscalls
    t_syscalls mock_syscalls = {
        .open = mock_open_fail,
        .fork = fork,  // Use real fork for this test
        // ... other syscalls
    };
    g_syscalls = &mock_syscalls;

    // Act & Assert
    // Test that your code handles open() failure correctly
    // ...
    
    // Cleanup: Restore real syscalls
    syscalls_init_real();
}
```

**Pros**:
- ✅ Full control in tests
- ✅ Easy to mock specific calls
- ✅ Works on all platforms
- ✅ Clean integration with Criterion

**Cons**:
- ❌ Requires code refactoring
- ❌ Extra indirection (minimal performance hit)

---

### Option 3: CMocka (C Mocking Framework)

**How it works**: Use a mocking framework designed for C.

```c
// tests/unit/test_with_cmocka.c
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// Mock open() - will be linked instead of real open()
int __wrap_open(const char *pathname, int flags, ...) {
    check_expected(pathname);
    return (int)mock();
}

static void test_open_permission_denied(void **state) {
    // Expect open() to be called with "infile-no-perms"
    expect_string(__wrap_open, pathname, "infile-no-perms");
    
    // Make mock return -1 with EACCES
    will_return(__wrap_open, -1);
    errno = EACCES;

    // Test your code
    // ...
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_open_permission_denied),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
```

```makefile
# Compile with --wrap flag to intercept functions
test_cmocka: tests/unit/test_with_cmocka.c
	$(CC) -Wl,--wrap=open -Wl,--wrap=fork $(CFLAGS) $< -lcmocka -o $@
```

**Pros**:
- ✅ Designed for C testing
- ✅ Rich assertion library
- ✅ Built-in mocking

**Cons**:
- ❌ Another framework to learn
- ❌ GNU ld specific (--wrap flag)
- ❌ May not work on macOS easily

---

## 🐍 Python Approach (Recommended for Minishell Integration Tests)

For minishell, **Python + subprocess mocking** is actually the best approach for integration tests.

### Why Python for Minishell?

1. **Better test organization** (fixtures, parameterization)
2. **Easy mocking** of subprocess behavior
3. **Cross-platform** (works in Docker, campus, macOS)
4. **Rich assertion library**
5. **Better error reporting**

### Example: Testing Pipe Behavior with Python

```python
# tests/integration/test_minishell.py
import pytest
import subprocess
from unittest.mock import patch, MagicMock
import os
import errno

class TestMinishell:
    
    @pytest.fixture
    def minishell(self):
        """Fixture to compile and provide minishell binary"""
        subprocess.run(["make", "re"], check=True)
        return "./minishell"
    
    def test_basic_pipe(self, minishell):
        """Test: echo hello | cat"""
        proc = subprocess.Popen(
            [minishell],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        stdout, stderr = proc.communicate(input="echo hello | cat\nexit\n")
        assert "hello" in stdout
        assert proc.returncode == 0
    
    def test_multiple_pipes(self, minishell):
        """Test: cat file | grep pattern | wc -l | cat"""
        # Create test file
        with open("test_file.txt", "w") as f:
            f.write("line1\npattern\nline3\npattern\n")
        
        proc = subprocess.Popen(
            [minishell],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        stdout, stderr = proc.communicate(
            input="cat test_file.txt | grep pattern | wc -l\nexit\n"
        )
        
        assert "2" in stdout
        os.unlink("test_file.txt")
    
    def test_permission_denied_portable(self, minishell):
        """Test permission denied (works in Docker and campus)"""
        # Create file with no permissions
        test_file = "no_perms.txt"
        with open(test_file, "w") as f:
            f.write("test")
        
        # Set permissions (works even as root in Docker)
        os.chmod(test_file, 0o000)
        
        try:
            # Try to read with minishell
            proc = subprocess.Popen(
                [minishell],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            
            stdout, stderr = proc.communicate(
                input=f"cat {test_file}\nexit\n"
            )
            
            # Even if root can read, we check the behavior
            # Or we can mock the file system
            assert "Permission denied" in stderr or proc.returncode != 0
        finally:
            os.chmod(test_file, 0o644)
            os.unlink(test_file)
    
    def test_fork_failure_simulation(self, minishell):
        """Simulate fork failure using resource limits"""
        import resource
        
        # Limit number of processes (Unix only)
        soft, hard = resource.getrlimit(resource.RLIMIT_NPROC)
        resource.setrlimit(resource.RLIMIT_NPROC, (10, hard))
        
        try:
            proc = subprocess.Popen(
                [minishell],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            
            # Try to execute command that requires fork
            stdout, stderr = proc.communicate(
                input="cat file | cat | cat | cat | cat\nexit\n",
                timeout=5
            )
            
            # Should handle fork failure gracefully
            assert proc.returncode != 0 or "fork" in stderr.lower()
        finally:
            resource.setrlimit(resource.RLIMIT_NPROC, (soft, hard))

    @pytest.mark.parametrize("cmd,expected", [
        ("echo hello", "hello"),
        ("echo hello | cat", "hello"),
        ("echo hello | cat | cat", "hello"),
        ("ls | wc -l", lambda x: x.isdigit()),
    ])
    def test_commands_parametrized(self, minishell, cmd, expected):
        """Parameterized tests for multiple commands"""
        proc = subprocess.Popen(
            [minishell],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        stdout, stderr = proc.communicate(input=f"{cmd}\nexit\n")
        
        if callable(expected):
            assert expected(stdout.strip())
        else:
            assert expected in stdout


# Advanced: Mocking subprocess for unit-like tests
class TestMinishellBuiltins:
    """Test builtins without actually executing shell"""
    
    @patch('subprocess.Popen')
    def test_cd_builtin(self, mock_popen):
        """Test cd builtin (doesn't fork)"""
        # Your cd implementation should not call fork
        # Test it separately
        pass
    
    @patch('os.fork')
    def test_handles_fork_failure(self, mock_fork):
        """Test fork failure handling"""
        mock_fork.side_effect = OSError(errno.EAGAIN, "Resource temporarily unavailable")
        
        # Test your fork wrapper
        # Should handle EAGAIN gracefully
        pass
```

### Running Python Tests

```bash
# Install pytest
pip install pytest pytest-cov

# Run tests
pytest tests/integration/ -v

# Run with coverage
pytest tests/integration/ --cov=. --cov-report=html

# Run specific test
pytest tests/integration/test_minishell.py::TestMinishell::test_basic_pipe -v

# Run in Docker
docker run -v $(pwd):/app -w /app python:3.11 pytest tests/integration/
```

---

## 🎯 Recommended Strategy for Minishell

### Three-Tier Testing Strategy

```
┌─────────────────────────────────────────────────────────┐
│ E2E Tests (Python + Real Shell)                    10% │
│ - Full shell execution                                  │
│ - Valgrind checks                                       │
│ - Performance tests                                     │
├─────────────────────────────────────────────────────────┤
│ Integration Tests (Python + subprocess)             50% │
│ - Command execution                                     │
│ - Pipe chains                                           │
│ - Redirections                                          │
│ - Signal handling                                       │
│ - Error scenarios                                       │
├─────────────────────────────────────────────────────────┤
│ Unit Tests (Criterion + Function Pointers)          40% │
│ - Parsing logic (tokenizer, lexer)                     │
│ - Environment variable expansion                        │
│ - Path resolution                                       │
│ - Builtin implementations                               │
│ - With mocked syscalls for edge cases                   │
└─────────────────────────────────────────────────────────┘
```

### What to Mock vs What to Test Real

| Component | Mock? | Tool | Reason |
|-----------|-------|------|--------|
| **Parsing/Tokenizing** | ❌ No | Criterion | Pure logic, no syscalls |
| **Environment expansion** | ❌ No | Criterion | Pure logic |
| **Builtin: cd, echo, env** | ❌ No | Criterion | Minimal syscalls |
| **Builtin: export, unset** | ❌ No | Criterion | In-process only |
| **Builtin: exit** | ✅ Yes | Criterion + mock | Calls exit() |
| **Fork/exec** | ✅ Yes (for failures) | Python | Test edge cases |
| **Pipe creation** | ✅ Yes (for failures) | Python | Test FD exhaustion |
| **Signal handling** | ✅ Yes | Python | Controlled signals |
| **File permissions** | ✅ Yes | Python | Docker compatibility |
| **Full command execution** | ❌ No | Python | Integration test |

---

## 🔧 Solving Your Specific Docker Problem

### Problem: Root User Bypasses Permissions

```bash
# This doesn't work in Docker as root
chmod 000 infile-no-perms
```

### Solution 1: Use Python to Verify Behavior

```python
# tests/integration/test_permissions.py
import pytest
import subprocess
import os

def test_permission_denied_portable():
    """Test that works in Docker and campus"""
    # Create test file
    test_file = "no_perms_test.txt"
    with open(test_file, "w") as f:
        f.write("secret data")
    
    # Remove permissions
    os.chmod(test_file, 0o000)
    
    try:
        # Test with python first (to verify setup)
        try:
            with open(test_file, "r") as f:
                f.read()
            is_root = True  # Opened successfully = we're root
        except PermissionError:
            is_root = False
        
        # Run pipex
        result = subprocess.run(
            ["./pipex", test_file, "cat", "wc", "outfile"],
            capture_output=True,
            text=True
        )
        
        if is_root:
            # In Docker as root: Skip or expect different behavior
            pytest.skip("Running as root, permission tests not applicable")
        else:
            # Normal user: Expect permission denied
            assert "Permission denied" in result.stderr
            assert result.returncode != 0
    finally:
        os.chmod(test_file, 0o644)
        os.unlink(test_file)
```

### Solution 2: Run Docker as Non-Root User

```dockerfile
# ci/containers/Dockerfile
FROM debian:bullseye

# Create non-root user
RUN useradd -m -s /bin/bash testuser

# Install dependencies
RUN apt-get update && apt-get install -y \
    gcc make valgrind python3 python3-pip

# Switch to non-root user
USER testuser
WORKDIR /home/testuser/pipex

# Copy and build
COPY --chown=testuser:testuser . .
RUN make re

# Run tests as non-root
CMD ["make", "test"]
```

```bash
# Run tests in Docker as non-root
docker build -t pipex-test -f ci/containers/Dockerfile .
docker run --rm pipex-test
```

### Solution 3: Use Linux Capabilities (Advanced)

```bash
# Remove CAP_DAC_OVERRIDE from container (removes root permission bypass)
docker run --rm --cap-drop=DAC_OVERRIDE pipex-test
```

---

## 📚 Recommended Approach for You

### For Finishing Pipex
**Current state**: ✅ Good enough
- Keep Criterion for `path_from_cmdname()`
- Keep bash for integration tests
- Fix Docker tests by running as non-root user (Solution 2)

### For Starting Minishell
**Recommended**:

1. **Setup Phase** (Week 1)
   ```bash
   # Install Python testing tools
   pip install pytest pytest-cov pytest-timeout
   
   # Create test structure
   mkdir -p tests/{unit,integration,e2e}
   ```

2. **Development Phase** (Weeks 2-8)
   - **TDD with unit tests** (Criterion)
     - Write tests for parser/tokenizer FIRST
     - Write tests for builtins
     - Mock syscalls using function pointers for edge cases
   
   - **Integration tests** (Python)
     - Write integration tests for each feature
     - Test pipe chains, redirections, signals
     - Use pytest fixtures for common setup

3. **Testing Strategy**
   ```
   Unit Tests (Criterion + Mocks)     40%
   ├── Tokenizer/Lexer
   ├── Parser
   ├── Environment variable expansion
   ├── Builtins (cd, echo, env, export, unset)
   └── Path resolution
   
   Integration Tests (Python)         50%
   ├── Command execution
   ├── Pipe chains (2, 3, 4+ pipes)
   ├── Redirections (<, >, >>)
   ├── Here-doc (<<)
   ├── Signal handling (Ctrl-C, Ctrl-D)
   ├── Error scenarios
   └── Edge cases
   
   E2E Tests (Python + Valgrind)      10%
   ├── Real-world command sequences
   ├── Memory leak detection
   └── Performance tests
   ```

---

## 🎓 Final Recommendations

### For Pipex (Now)
1. ❌ **Don't mock** - Not worth it for such a simple project
2. ✅ **Fix Docker tests** - Run container as non-root user
3. ✅ **Keep current strategy** - It's good enough

### For Minishell (Future)
1. ✅ **DO mock syscalls** - Use function pointers for unit tests
2. ✅ **Use Python** - For integration tests (pytest is amazing)
3. ✅ **Test-first approach** - Write tests before implementation
4. ✅ **Three-tier strategy** - Unit (40%) + Integration (50%) + E2E (10%)

### Tools to Learn Before Minishell
```bash
# Python testing
pip install pytest pytest-cov
pytest --help

# Learn pytest basics
# https://docs.pytest.org/en/stable/

# Mocking in Python
# https://docs.python.org/3/library/unittest.mock.html
```

---

## 📖 Further Reading

- **Function Pointers in C**: For dependency injection pattern
- **Pytest Documentation**: Essential for minishell testing
- **LD_PRELOAD Tutorial**: For advanced mocking
- **Test Driven Development in C**: Testing embedded/systems code

---

**Bottom Line**: 
- Pipex: Mocking not worth it ❌
- Minishell: Mocking essential ✅
- Use Python for minishell integration tests
- Use function pointers for mockable C unit tests
- Fix Docker by running as non-root user

Your instinct is correct: for complex multiprocess projects like minishell, you WILL need mocking and better testing tools. Python + pytest is your best friend here.

