# Testing Strategy for Pipex

## Overview

Pipex is a **systems programming project** with heavy multiprocess architecture (fork, pipe, exec). This creates unique testing challenges because traditional unit testing frameworks struggle with:
- Process isolation
- System call side effects
- Inter-process communication
- Signal handling and exit statuses

## Testing Pyramid for Pipex

```
                /\
               /  \
              / E2E\         (Bash/Python - Full binary testing)
             /------\
            /  Inte- \       (Bash/Python - Module interaction)
           /  gration \
          /------------\
         /    Unit      \    (Criterion - Pure functions only)
        /________________\
```

## Unit Tests (Criterion - ~30% of tests)

### Definition
Unit tests verify **individual functions in isolation** that:
- Have **no multiprocess behavior** (no fork/exec)
- Have **predictable, deterministic output**
- Can be tested **without real system calls** (or with minimal ones like `access()`)
- Are **pure or nearly pure functions**

### What to Unit Test in Pipex

#### ✅ Good Unit Test Candidates

1. **`path_from_cmdname()`** ✓ (You already have this!)
   - Pure function logic
   - String manipulation
   - PATH parsing
   - Uses `access()` but doesn't affect system state

2. **String parsing/validation functions**
   ```c
   // Example: If you had argument parsing
   char **parse_command_args(char *cmd);
   int validate_filename(char *filename);
   ```

3. **Error message formatting** (if extracted)
   ```c
   // If you refactor error.c to separate formatting from exit:
   char *format_error_message(char *context, char *description);
   ```

4. **Helper utilities**
   ```c
   // Example from path_from_cmdname.c
   char *const *get_path_envp(char *const envp[]);
   void ft_split_free(char **paths);
   ```

#### ❌ Bad Unit Test Candidates (Move to Integration)

1. **`child1_cmd1()`, `child2_cmd2()`**
   - Reason: Call `execve()`, `dup2()`, `open()` with real files
   - These are **integration points** with the OS

2. **`callexecve1()`, `callexecve2()`**
   - Reason: Call `execve()` which replaces the process
   - Cannot be tested without mocking the entire OS

3. **`xfork()`, `xclose()`**
   - Reason: Thin wrappers around system calls
   - Testing these is testing the OS, not your logic

4. **`open_infile1()`, `open_infile2()`**
   - Reason: Require real file I/O
   - Should be tested via integration tests

### Unit Test Examples

```c
// tests/unit/test_utils.c
#include <criterion/criterion.h>
#include "../../include/utils.h"

Test(ft_split_free, handles_null) {
    ft_split_free(NULL);  // Should not crash
    cr_assert(1);
}

Test(get_path_envp, finds_path) {
    char *envp[] = {"HOME=/home", "PATH=/usr/bin", NULL};
    char *const *result = get_path_envp(envp);
    cr_assert_str_eq(*result, "PATH=/usr/bin");
}

Test(get_path_envp, no_path_returns_empty) {
    char *envp[] = {"HOME=/home", "USER=test", NULL};
    char *const *result = get_path_envp(envp);
    cr_assert_null(*result);
}
```

---

## Integration Tests (Bash/Python - ~60% of tests)

### Definition
Integration tests verify **interactions between modules** and **system integration**:
- Multiple functions working together
- Real system calls (fork, pipe, exec, open)
- File I/O with real files
- Process communication
- Exit status propagation

### Current Integration Tests ✓

Your `test_runner.sh` is **perfect for integration testing**:
- Tests the full pipex binary
- Uses real files (`infile`, `outfile`)
- Verifies stdout/stderr
- Checks exit codes
- Compares behavior with shell pipes

### Integration Test Organization

```
tests/
├── integration/          # NEW: Dedicated integration tests
│   ├── test_pipe_chain.sh
│   ├── test_error_handling.sh
│   └── test_file_permissions.sh
├── input/               # Your current integration test data
│   ├── 00_basic.in
│   ├── 00_basic.exp
│   └── ...
└── unit/                # Criterion tests for pure functions
    └── test_path_from_cmdname.c
```

### What to Integration Test

1. **Full pipeline execution**
   ```bash
   ./pipex infile "grep pattern" "wc -l" outfile
   # Compare with: < infile grep pattern | wc -l > outfile
   ```

2. **Error scenarios** (You already have these!)
   - Command not found (127 exit code)
   - No permission (126 exit code)
   - File doesn't exist
   - Invalid arguments

3. **Edge cases**
   - Empty commands
   - Commands with spaces
   - Special characters
   - Large files

4. **System call interactions**
   - Pipe buffering
   - FD inheritance
   - Signal handling

---

## E2E Tests (Bash/Python - ~10% of tests)

### Definition
End-to-end tests verify **complete user scenarios**:
- Real-world use cases
- Performance under load
- Memory leaks (valgrind)
- Behavior across different environments

### Current E2E Tests ✓

You already have these:
- `valgrind_suite.sh` - Memory leak detection
- Manual tests with real commands

---

## Answering Your Specific Questions

### Q1: "1 file 1 function" rule?

**Answer**: This is too strict for systems programming. Better rule:

> **Unit test = Function with deterministic output and no process/system state changes**

Examples:
- ✅ `path_from_cmdname()` - deterministic, no state changes
- ❌ `child1_cmd1()` - calls `execve()`, changes process state

### Q2: When does it become integration?

**Answer**: The moment you:
1. Use `fork()` or `exec()` family
2. Create/modify real files
3. Use pipes for IPC
4. Test interaction between modules (different directories)
5. Depend on exit codes from child processes

### Q3: Criterion not working for integration tests?

**Answer**: Correct! Criterion (and most unit test frameworks) are designed for:
- Single-process testing
- Deterministic, repeatable tests
- Fast execution

Criterion **cannot handle**:
- `fork()` in tests (process tree confusion)
- `execve()` (replaces the test process)
- Real file I/O (side effects between tests)

**Solution**: Use Criterion **only** for pure functions. Use bash/Python for everything else.

### Q4: Testing system calls = integration?

**Answer**: **Yes, almost always**. Here's the rule:

| System Call | Unit Test? | Integration Test? | Why |
|-------------|------------|-------------------|-----|
| `access()`, `stat()` | Maybe | Usually | Read-only, minimal side effects |
| `open()`, `read()`, `write()` | No | Yes | Real file I/O, side effects |
| `fork()`, `pipe()` | Never | Yes | Multiprocess behavior |
| `execve()` | Never | Yes | Replaces process |
| `dup2()`, `close()` | No | Yes | FD manipulation has side effects |

**Exception**: If you mock/stub system calls (advanced), but this is often not worth it.

---

## Recommended Testing Strategy

### Phase 1: Extract Testable Functions (Refactor)

Identify logic that can be extracted into pure functions:

```c
// Before (in child1_cmd1.c - hard to unit test)
void callexecve1(char *argv, char *const envp[]) {
    char **args = ft_split(argv, ' ');
    // ... logic ...
}

// After (extract to utils - easy to unit test)
char **parse_command_args(char *cmd) {
    if (!cmd || cmd[0] == '\0')
        return NULL;
    return ft_split(cmd, ' ');
}

// Test with Criterion
Test(parse_command_args, empty_string) {
    char **result = parse_command_args("");
    cr_assert_null(result);
}
```

### Phase 2: Organize Tests by Type

```
tests/
├── unit/                    # Criterion - Pure functions
│   ├── test_path_from_cmdname.c
│   ├── test_argument_parsing.c
│   └── test_error_formatting.c
├── integration/             # Bash - Module interaction
│   ├── test_child_processes.sh
│   ├── test_pipe_communication.sh
│   └── test_fd_handling.sh
└── e2e/                     # Bash/Python - Full scenarios
    ├── test_runner.sh       # Your current test runner
    └── test_valgrind.sh
```

### Phase 3: Testing Pyramid Distribution

For a project like pipex:
- **30% Unit Tests** (Criterion) - Fast feedback on logic
- **60% Integration Tests** (Bash) - Core functionality
- **10% E2E Tests** (Bash/Valgrind) - Real-world scenarios

This is **inverted** from typical applications because:
- Most of your code **is** integration with the OS
- The "business logic" is minimal (PATH resolution, arg parsing)
- The value is in **correct multiprocess behavior**

---

## Tooling Recommendations

### For Unit Tests: Criterion ✓

**Keep using Criterion** for:
- `path_from_cmdname()` and similar pure functions
- String manipulation
- Validation logic

```makefile
# Your current Makefile setup is good!
unit: $(UNIT_BIN)
	./$@ --color --fail-fast
```

### For Integration Tests: Bash ✓

**Keep using bash** for:
- Full pipex execution
- File I/O testing
- Exit code verification

Your `test_runner.sh` is excellent! Consider adding:
- Timeout handling (to catch infinite loops)
- Better diff output
- Test isolation (cleanup between tests)

### For E2E Tests: Consider Python

Python can provide better test organization:

```python
# tests/e2e/test_pipex.py
import subprocess
import pytest

def test_basic_pipe():
    result = subprocess.run(
        ["./pipex", "infile", "cat", "wc -l", "outfile"],
        capture_output=True
    )
    assert result.returncode == 0
    with open("outfile") as f:
        assert f.read().strip() == "42"

def test_command_not_found():
    result = subprocess.run(
        ["./pipex", "infile", "nonexistent", "cat", "outfile"],
        capture_output=True
    )
    assert result.returncode == 127
    assert b"command not found" in result.stderr
```

**Benefits**:
- Better test organization (fixtures, parameterization)
- Easier to maintain complex test scenarios
- Better error reporting
- Cross-platform (if needed)

---

## Practical Implementation Plan

### Step 1: Categorize Existing Tests ✓

You already have:
- ✅ Unit: `test_path_from_cmdname.c`
- ✅ Integration/E2E: `test_runner.sh`

### Step 2: Identify More Unit Test Candidates

Review your code for pure functions:
```bash
# Look for functions that don't call fork/exec/open
grep -r "^[a-z_]*\s*[a-z_]*(" src/ | grep -v "fork\|exec\|open\|dup2"
```

### Step 3: Refactor for Testability (Optional)

Extract testable logic from child processes:
- Command parsing
- Argument validation  
- Error message formatting

### Step 4: Document Test Types

Add comments to test files:
```c
// tests/unit/test_path_from_cmdname.c
/** 
 * UNIT TEST: Tests path resolution logic in isolation.
 * No multiprocess behavior, no real exec calls.
 */
```

```bash
# tests/integration/test_runner.sh
##
# INTEGRATION TEST: Tests full pipex binary with real processes.
# Tests fork, pipe, exec, and file I/O.
##
```

### Step 5: Update Makefile Targets

```makefile
# Make test targets explicit
test-unit: $(UNIT_BIN)
	@echo "$(CYAN)[unit tests]$(CLR_RMV) Criterion"
	./$(UNIT_BIN) --color --fail-fast

test-integration:
	@echo "$(CYAN)[integration tests]$(CLR_RMV) bash test_runner.sh"
	bash tests/test_runner.sh "$(PIPEX_BIN)" "$(TEST_INPUT_DIR)" "$(TEST_OUTPUT_DIR)"

test-e2e: valgrind
	@echo "$(CYAN)[e2e tests]$(CLR_RMV) valgrind + memory checks"

test-all: test-unit test-integration test-e2e
```

---

## Summary: Clear Rules for Pipex

### Unit Test (Criterion)
- ✅ Pure functions (deterministic output)
- ✅ String manipulation, parsing, validation
- ✅ Logic that can be extracted from system calls
- ✅ PATH resolution (`path_from_cmdname`)
- ❌ Anything with fork/exec/pipe
- ❌ Real file I/O

### Integration Test (Bash/Python)
- ✅ Full pipex binary execution
- ✅ Real process creation (fork)
- ✅ Real command execution (exec)
- ✅ Real file operations
- ✅ Pipe communication between processes
- ✅ Exit code propagation
- ✅ Error handling across modules

### E2E Test (Bash/Valgrind)
- ✅ Real-world scenarios
- ✅ Memory leak detection
- ✅ Performance testing
- ✅ Cross-environment validation

---

## Final Recommendation

**Your current approach is mostly correct!** Just needs organization:

1. **Keep Criterion** for `path_from_cmdname()` and any other pure functions
2. **Keep bash** for testing the pipex binary (this IS integration testing)
3. **Don't force unit tests** where they don't fit - systems programming is about integration
4. **Consider Python** only if bash tests become unwieldy (>1000 lines)

The key insight: **In a multiprocess project like pipex, most of your tests WILL be integration tests, and that's OK!** The value is in testing that fork, pipe, and exec work together correctly, not in isolating individual system calls.

