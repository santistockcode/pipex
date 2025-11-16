# Testing Quick Reference Card

## 🎯 Quick Decision Tree

```
Does the function call fork(), exec(), or pipe()?
├─ YES → Integration Test (bash)
└─ NO
    ├─ Does it open/read/write real files?
    │   ├─ YES → Integration Test (bash)
    │   └─ NO
    │       ├─ Does it have deterministic output for given input?
    │       │   ├─ YES → Unit Test (Criterion) ✓
    │       │   └─ NO → Integration Test (bash)
```

## 📋 Test Type Checklist

### Unit Test (Criterion)
- [ ] Function is pure or nearly pure
- [ ] No fork/exec/pipe calls
- [ ] No real file I/O (open/read/write)
- [ ] Deterministic output
- [ ] Can run in isolation
- [ ] Fast (< 1ms)

**Examples**: `path_from_cmdname`, `ft_split_free`, `get_path_envp`

### Integration Test (Bash)
- [ ] Tests multiple modules together
- [ ] Uses real system calls
- [ ] Creates/modifies files
- [ ] Tests process communication
- [ ] Verifies exit codes
- [ ] May be slower (> 10ms)

**Examples**: Full pipex execution, pipe communication, FD handling

### E2E Test (Bash/Valgrind)
- [ ] Tests complete user scenario
- [ ] Real-world use case
- [ ] Memory leak detection
- [ ] Performance testing
- [ ] Cross-environment validation

**Examples**: Valgrind suite, stress tests

## 🛠️ Commands

```bash
# Run all tests
make test

# Run only unit tests (fast, for TDD)
make test-unit

# Run only integration tests
make test-integration

# Run E2E tests (slow, for CI/CD)
make test-e2e

# Debug a specific unit test
make unit-debug TEST=path_from_cmdname/1 PORT=1234
# In another terminal:
make attach-gdb PORT=1234
```

## 📝 Adding New Tests

### Adding a Unit Test

1. **Identify a pure function** (no fork/exec/file I/O)
2. **Create or edit** `tests/unit/test_<module>.c`
3. **Write test**:
   ```c
   Test(module, test_name) {
       // Arrange
       // Act
       // Assert
   }
   ```
4. **Run**: `make test-unit`

### Adding an Integration Test

1. **Create test input**: `tests/input/NN_description.in`
   ```
   infile "cmd1" "cmd2" outfile
   ```
2. **Create expected output**: `tests/input/NN_description.exp`
3. **Create expected status** (optional): `tests/input/NN_description.status`
4. **Run**: `make test-integration`

## 🎨 Test Examples

### Good Unit Test
```c
// tests/unit/test_utils.c
Test(ft_split_free, null_pointer) {
    ft_split_free(NULL);  // Should not crash
    cr_assert(1);  // If we get here, test passed
}
```

### Good Integration Test
```bash
# tests/input/08_pipe_large_data.in
large_file "cat" "wc -l" outfile

# tests/input/08_pipe_large_data.exp
1000000
```

## 🚫 Common Mistakes

### ❌ Don't Unit Test System Calls
```c
// BAD: Testing open() is testing the OS
Test(open_file, basic) {
    int fd = open("test.txt", O_RDONLY);
    cr_assert_neq(fd, -1);
}
```

### ✅ Do Integration Test System Calls
```bash
# GOOD: Test via full pipex execution
./pipex nonexistent "cat" "wc" outfile
# Verify error message and exit code
```

### ❌ Don't Unit Test Functions That Call Exec
```c
// BAD: callexecve1 will replace the test process
Test(callexecve1, basic) {
    callexecve1("ls", envp);  // Process dies here!
}
```

### ✅ Do Extract Testable Logic
```c
// GOOD: Extract parsing logic
char **parse_command(char *cmd) {
    if (!cmd || cmd[0] == '\0') return NULL;
    return ft_split(cmd, ' ');
}

Test(parse_command, empty_string) {
    char **result = parse_command("");
    cr_assert_null(result);
}
```

## 📊 Coverage Goals

| Test Type | Coverage % | Speed | Tool |
|-----------|-----------|-------|------|
| Unit | 30% | Fast (ms) | Criterion |
| Integration | 60% | Medium (s) | Bash |
| E2E | 10% | Slow (min) | Valgrind |

**Note**: Inverted pyramid is normal for systems programming!

## 🔍 When in Doubt

**Ask yourself**: 
> "If I run this test 100 times in parallel, will it always pass?"

- **YES** → Probably a unit test
- **NO** → Probably an integration test

**Remember**: In pipex, most tests WILL be integration tests. That's OK!

