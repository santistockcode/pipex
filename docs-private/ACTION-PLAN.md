# Action Plan: Fix Docker Tests & Prepare for Minishell

**Goal**: 
1. Fix permission tests in Docker (NOW)
2. Prepare testing strategy for minishell (SOON)

---

## 🔥 Immediate Fix: Docker Permission Tests (30 minutes)

### Problem
Your `05_open_no_perms` test fails in Docker because Docker runs as root by default, bypassing file permissions.

### Solution: Run Docker as Non-Root User

#### Update Dockerfile

```dockerfile
# ci/containers/Dockerfile
FROM debian:bullseye

# Install dependencies first
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    valgrind \
    libcriterion-dev \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Create a non-root user
RUN useradd -m -u 1000 -s /bin/bash testuser

# Switch to non-root user
USER testuser
WORKDIR /home/testuser/app

# Now all commands run as testuser (not root)
```

#### Update docker-compose.yml (if you use it)

```yaml
# docker-compose.yml
version: '3.8'
services:
  pipex-test:
    build:
      context: .
      dockerfile: ci/containers/Dockerfile
    volumes:
      - .:/home/testuser/app
    user: "1000:1000"  # Run as non-root
    command: make test
```

#### Or: Run Docker with User Flag

```bash
# Build image
docker build -t pipex-test -f ci/containers/Dockerfile .

# Run as non-root user
docker run --rm --user 1000:1000 -v $(pwd):/home/testuser/app -w /home/testuser/app pipex-test make test
```

#### Or: Quick Fix in test_runner.sh

```bash
# tests/test_runner.sh
# Add at the top:

# Check if running as root
if [ "$EUID" -eq 0 ]; then
    echo "⚠️  Running as root - skipping permission tests"
    # Skip permission-based tests or mark them as expected to pass
fi
```

---

## 🎯 For Minishell: Recommended Testing Strategy

### Phase 1: Setup (Week 1 - Before Coding)

#### 1.1 Install Python Testing Tools

```bash
# On your machine
pip3 install pytest pytest-cov pytest-timeout

# In Docker
# Add to Dockerfile:
RUN pip3 install pytest pytest-cov pytest-timeout
```

#### 1.2 Create Test Structure

```bash
mkdir -p minishell/tests/{unit,integration,e2e,fixtures}
```

```
minishell/
├── src/
│   ├── parser/
│   ├── executor/
│   ├── builtins/
│   └── utils/
├── tests/
│   ├── unit/           # Criterion tests
│   │   ├── test_tokenizer.c
│   │   ├── test_parser.c
│   │   ├── test_builtins.c
│   │   └── mocks/
│   │       └── mock_syscalls.c
│   ├── integration/    # Python tests
│   │   ├── conftest.py           # Pytest fixtures
│   │   ├── test_pipes.py
│   │   ├── test_redirections.py
│   │   ├── test_signals.py
│   │   └── test_builtins_integration.py
│   ├── e2e/           # End-to-end tests
│   │   └── test_real_world_scenarios.py
│   └── fixtures/      # Test data files
│       ├── test_input.txt
│       └── test_script.sh
└── Makefile
```

#### 1.3 Create Pytest Configuration

```python
# tests/conftest.py
import pytest
import subprocess
import os
import tempfile
import shutil

@pytest.fixture(scope="session")
def minishell_bin():
    """Compile minishell once for all tests"""
    subprocess.run(["make", "re"], check=True, cwd="..")
    return "../minishell"

@pytest.fixture
def temp_dir():
    """Create temporary directory for each test"""
    tmpdir = tempfile.mkdtemp()
    yield tmpdir
    shutil.rmtree(tmpdir)

@pytest.fixture
def mock_env():
    """Provide clean environment for tests"""
    return {
        "PATH": "/usr/bin:/bin",
        "HOME": "/home/testuser",
        "USER": "testuser",
    }

@pytest.fixture
def minishell_process(minishell_bin):
    """Start minishell process for interactive tests"""
    def _start(input_text=""):
        proc = subprocess.Popen(
            [minishell_bin],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        stdout, stderr = proc.communicate(input=input_text, timeout=5)
        return stdout, stderr, proc.returncode
    
    return _start
```

### Phase 2: Development with TDD (Weeks 2-8)

#### 2.1 Write Test FIRST (Example: Tokenizer)

```c
// tests/unit/test_tokenizer.c
#include <criterion/criterion.h>
#include "../../include/tokenizer.h"

Test(tokenizer, simple_command) {
    char *input = "ls -la";
    t_token *tokens = tokenize(input);
    
    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0].value, "ls");
    cr_assert_eq(tokens[0].type, TOKEN_WORD);
    cr_assert_str_eq(tokens[1].value, "-la");
    cr_assert_eq(tokens[1].type, TOKEN_WORD);
    
    free_tokens(tokens);
}

Test(tokenizer, pipe_operator) {
    char *input = "ls | grep test";
    t_token *tokens = tokenize(input);
    
    cr_assert_str_eq(tokens[0].value, "ls");
    cr_assert_str_eq(tokens[1].value, "|");
    cr_assert_eq(tokens[1].type, TOKEN_PIPE);
    cr_assert_str_eq(tokens[2].value, "grep");
    
    free_tokens(tokens);
}

Test(tokenizer, quoted_string) {
    char *input = "echo \"hello world\"";
    t_token *tokens = tokenize(input);
    
    cr_assert_str_eq(tokens[1].value, "hello world");
    cr_assert_eq(tokens[1].type, TOKEN_WORD);
    
    free_tokens(tokens);
}
```

#### 2.2 Write Implementation (TDD Cycle)

```c
// src/parser/tokenizer.c
t_token *tokenize(char *input) {
    // Implement to pass tests
    // Start simple, add complexity as tests demand
}
```

#### 2.3 Write Integration Test

```python
# tests/integration/test_pipes.py
import pytest

def test_simple_pipe(minishell_process):
    """Test: echo hello | cat"""
    stdout, stderr, code = minishell_process("echo hello | cat\nexit\n")
    
    assert "hello" in stdout
    assert code == 0

def test_multiple_pipes(minishell_process):
    """Test: cat file | grep pattern | wc -l"""
    # Create test file
    with open("test.txt", "w") as f:
        f.write("line1\npattern\nline3\npattern\n")
    
    stdout, stderr, code = minishell_process(
        "cat test.txt | grep pattern | wc -l\nexit\n"
    )
    
    assert "2" in stdout
    os.unlink("test.txt")

def test_pipe_failure_handling(minishell_process):
    """Test: cmd1 | nonexistent | cmd3"""
    stdout, stderr, code = minishell_process(
        "echo test | nonexistent_command | cat\nexit\n"
    )
    
    # Should show error but continue pipeline
    assert "command not found" in stderr.lower() or "not found" in stderr.lower()
```

### Phase 3: Mock System Calls for Edge Cases

#### 3.1 Create Syscall Interface (like refactoring example)

```c
// include/syscall_interface.h
typedef struct s_syscalls {
    int (*fork)(void);
    int (*pipe)(int[2]);
    // ... etc
} t_syscalls;

extern t_syscalls *g_syscalls;
```

#### 3.2 Test Edge Cases with Mocks

```c
// tests/unit/test_fork_failure.c
Test(executor, handles_fork_failure) {
    mock_syscalls_enable();
    mock_set_fork_failure(1);
    
    // Test that your executor handles fork() returning -1
    int result = execute_command("ls");
    
    cr_assert_eq(result, -1);
    // Verify error message was printed
    
    mock_syscalls_disable();
}

Test(executor, handles_pipe_failure) {
    mock_syscalls_enable();
    mock_set_pipe_failure(1);
    
    // Test pipeline creation when pipe() fails
    int result = execute_pipeline("ls | cat");
    
    cr_assert_eq(result, -1);
    
    mock_syscalls_disable();
}
```

---

## 📋 Cheat Sheet: When to Use What

### Use Criterion (Unit Tests) For:
```c
✅ Tokenizer/Lexer
✅ Parser (AST building)
✅ Environment variable expansion
✅ String utilities (quote removal, etc)
✅ Builtins (cd, echo, env, export, unset, exit)
✅ Path resolution
✅ Error message formatting

❌ Full command execution
❌ Signal handling
❌ Real file operations
❌ Multiprocess scenarios
```

### Use Python (Integration Tests) For:
```python
✅ Full minishell execution
✅ Pipe chains (2, 3, 4+ pipes)
✅ Redirections (<, >, >>)
✅ Here-doc (<<)
✅ Signal handling (Ctrl-C, Ctrl-D)
✅ Command sequences (multiple commands)
✅ Environment variable persistence
✅ Exit codes
✅ Error scenarios

❌ Testing pure parsing logic
❌ Testing string utilities
```

### Use Mocks For:
```c
✅ Testing fork() failure
✅ Testing pipe() failure
✅ Testing file permission errors (Docker-safe!)
✅ Testing resource exhaustion
✅ Testing signal handling edge cases
✅ Testing without side effects

❌ Testing actual command output
❌ Testing real file operations (use integration)
```

---

## 🚀 Quick Start Commands

### For Pipex (Now)

```bash
# Fix Docker tests
docker build -t pipex-test -f ci/containers/Dockerfile .
docker run --rm --user $(id -u):$(id -g) \
    -v $(pwd):/home/testuser/app \
    -w /home/testuser/app \
    pipex-test make test

# Or add to Makefile:
docker-test:
	docker run --rm --user $(id -u):$(id -g) \
	    -v $(pwd):/app -w /app pipex-test make test
```

### For Minishell (Soon)

```bash
# Unit tests (fast feedback)
make test-unit

# Integration tests (Python)
pytest tests/integration/ -v

# Specific test
pytest tests/integration/test_pipes.py::test_simple_pipe -v

# With coverage
pytest tests/integration/ --cov=src --cov-report=html

# E2E tests (with valgrind)
make test-e2e

# All tests
make test-all
```

---

## 📊 Testing Metrics for Minishell

### Minimum Viable Testing
- **Unit tests**: 20+ tests covering parser, tokenizer, builtins
- **Integration tests**: 30+ tests covering pipes, redirections, signals
- **E2E tests**: 5+ real-world scenarios
- **Total time**: Unit (< 1s), Integration (< 10s), E2E (< 1min)

### Good Testing Coverage
- **Unit tests**: 50+ tests
- **Integration tests**: 100+ tests
- **E2E tests**: 20+ scenarios
- **Code coverage**: > 80%

---

## 🎯 Final Recommendations

### For Pipex (This Week)
1. ✅ Fix Docker tests using non-root user (30 min)
2. ✅ Keep current testing strategy (it's fine!)
3. ❌ Don't refactor with mocks (not worth it for pipex)

### For Minishell (Next Project)
1. ✅ Use function pointers for mockable syscalls
2. ✅ Use Python + pytest for integration tests
3. ✅ Use Criterion for unit tests
4. ✅ Follow TDD: Write test → Write code → Refactor
5. ✅ Test edge cases with mocks (fork failure, pipe failure, etc)
6. ✅ Run tests in Docker as non-root from day 1

### Time Investment
- **Pipex fix**: 30 minutes
- **Minishell setup**: 4-5 hours (Week 1)
- **Minishell ongoing**: 20-30% of development time
- **Payoff**: Save weeks of debugging, catch bugs early, portable tests

---

## 📖 Next Steps

1. **Today**: Fix Docker tests for pipex
2. **This week**: Read Python pytest docs
3. **Before minishell**: Study the refactoring example
4. **Week 1 of minishell**: Set up test infrastructure
5. **Weeks 2-8**: TDD all the way

Your instinct is correct: for minishell, you WILL need:
- ✅ Mocking system calls
- ✅ Python for integration tests
- ✅ Better test infrastructure

Start learning pytest now, you'll thank yourself later! 🚀

