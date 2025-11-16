# Testing Examples for Pipex

## Example 1: Testing Path Resolution (Current ✓)

### Function (Pure Logic)
```c
// src/utils/path_from_cmdname.c
char *path_from_cmdname(char *arg, char *const envp[]) {
    // ... implementation
}
```

### Unit Test (Criterion)
```c
// tests/unit/test_path_from_cmdname.c
Test(path_from_cmdname, finds_command_in_path) {
    char *fake_envp[] = { 
        "PATH=/usr/bin:/bin", 
        NULL 
    };
    char *result = path_from_cmdname("ls", fake_envp);
    cr_assert_not_null(result);
    cr_expect_str_eq(result, "/usr/bin/ls");
    free(result);
}

Test(path_from_cmdname, returns_null_when_not_found) {
    char *fake_envp[] = { 
        "PATH=/usr/bin", 
        NULL 
    };
    char *result = path_from_cmdname("nonexistent_cmd", fake_envp);
    cr_assert_null(result);
}
```

**Why this works**: 
- No fork/exec
- Deterministic (same input → same output)
- Uses `access()` but doesn't modify system state

---

## Example 2: Refactoring for Testability

### Before: Hard to Test ❌

```c
// src/child1/child1_cmd1.c
void callexecve1(char *argv, char *const envp[]) {
    char **args;
    char *path;

    // All logic is entangled with execve
    if (!argv || argv[0] == '\0') {
        ft_putstr_fd("pipex: ", 2);
        ft_putstr_fd("Command '' not found\n", 2);
        exit(127);
    }
    args = ft_split(argv, ' ');
    if (access(args[0], 0) == 0)
        path = ft_strdup(args[0]);
    else
        path = path_from_cmdname(args[0], envp);
    if (path == NULL)
        custom_error_fd2(args[0], args, 127);
    
    execve(path, args, envp);  // ← Can't test past this point
    perror("execve failed");
    free(path);
    ft_split_free(args);
    exit(EXIT_FAILURE);
}
```

**Problem**: Can't unit test because `execve()` replaces the process.

### After: Extracted Testable Logic ✓

```c
// src/utils/command_parser.c (NEW)
typedef struct s_command {
    char    **args;
    char    *path;
    int     is_valid;
    int     error_code;
} t_command;

/**
 * Parse and resolve command - Pure function, no side effects
 * Returns: Command structure with resolved path
 */
t_command parse_and_resolve_command(char *argv, char *const envp[]) {
    t_command cmd = {NULL, NULL, 1, 0};

    // Validation
    if (!argv || argv[0] == '\0') {
        cmd.is_valid = 0;
        cmd.error_code = 127;
        return cmd;
    }

    // Parsing
    cmd.args = ft_split(argv, ' ');
    if (!cmd.args) {
        cmd.is_valid = 0;
        cmd.error_code = 127;
        return cmd;
    }

    // Path resolution
    if (access(cmd.args[0], F_OK) == 0)
        cmd.path = ft_strdup(cmd.args[0]);
    else
        cmd.path = path_from_cmdname(cmd.args[0], envp);

    if (cmd.path == NULL) {
        cmd.is_valid = 0;
        cmd.error_code = 127;
    }

    return cmd;
}

void free_command(t_command *cmd) {
    if (cmd->args)
        ft_split_free(cmd->args);
    if (cmd->path)
        free(cmd->path);
}
```

```c
// src/child1/child1_cmd1.c (REFACTORED)
void callexecve1(char *argv, char *const envp[]) {
    t_command cmd = parse_and_resolve_command(argv, envp);

    if (!cmd.is_valid) {
        if (cmd.args)
            custom_error_fd2(cmd.args[0], cmd.args, cmd.error_code);
        else
            ft_putstr_fd("pipex: Command '' not found\n", 2);
        free_command(&cmd);
        exit(cmd.error_code);
    }

    // Now just execute
    if (execve(cmd.path, cmd.args, envp) == -1)
        perror("execve failed");
    
    free_command(&cmd);
    exit(EXIT_FAILURE);
}
```

### Unit Tests for Refactored Code ✓

```c
// tests/unit/test_command_parser.c
#include <criterion/criterion.h>
#include "../../include/command_parser.h"

Test(parse_and_resolve_command, valid_command_in_path) {
    char *fake_envp[] = {"PATH=/usr/bin:/bin", NULL};
    
    t_command cmd = parse_and_resolve_command("ls -la", fake_envp);
    
    cr_assert_eq(cmd.is_valid, 1);
    cr_assert_not_null(cmd.args);
    cr_assert_str_eq(cmd.args[0], "ls");
    cr_assert_str_eq(cmd.args[1], "-la");
    cr_assert_not_null(cmd.path);
    cr_expect_str_eq(cmd.path, "/usr/bin/ls");
    
    free_command(&cmd);
}

Test(parse_and_resolve_command, empty_command) {
    char *fake_envp[] = {"PATH=/usr/bin", NULL};
    
    t_command cmd = parse_and_resolve_command("", fake_envp);
    
    cr_assert_eq(cmd.is_valid, 0);
    cr_assert_eq(cmd.error_code, 127);
    cr_assert_null(cmd.args);
    cr_assert_null(cmd.path);
    
    free_command(&cmd);
}

Test(parse_and_resolve_command, command_not_found) {
    char *fake_envp[] = {"PATH=/usr/bin", NULL};
    
    t_command cmd = parse_and_resolve_command("nonexistent", fake_envp);
    
    cr_assert_eq(cmd.is_valid, 0);
    cr_assert_eq(cmd.error_code, 127);
    cr_assert_not_null(cmd.args);
    cr_assert_str_eq(cmd.args[0], "nonexistent");
    cr_assert_null(cmd.path);
    
    free_command(&cmd);
}

Test(parse_and_resolve_command, absolute_path) {
    char *fake_envp[] = {"PATH=/usr/bin", NULL};
    
    // Create temp file for testing
    system("touch /tmp/test_cmd && chmod +x /tmp/test_cmd");
    
    t_command cmd = parse_and_resolve_command("/tmp/test_cmd", fake_envp);
    
    cr_assert_eq(cmd.is_valid, 1);
    cr_assert_str_eq(cmd.path, "/tmp/test_cmd");
    
    free_command(&cmd);
    system("rm /tmp/test_cmd");
}

Test(parse_and_resolve_command, relative_path) {
    char *fake_envp[] = {"PATH=/usr/bin", NULL};
    
    system("touch ./local_cmd && chmod +x ./local_cmd");
    
    t_command cmd = parse_and_resolve_command("./local_cmd", fake_envp);
    
    cr_assert_eq(cmd.is_valid, 1);
    cr_assert_str_eq(cmd.path, "./local_cmd");
    
    free_command(&cmd);
    system("rm ./local_cmd");
}
```

**Benefits of Refactoring**:
1. ✅ Logic is testable without `execve()`
2. ✅ Can test all edge cases (empty, not found, etc.)
3. ✅ Separation of concerns (parsing vs execution)
4. ✅ Tests run fast (no process creation)
5. ✅ Can run tests in parallel

---

## Example 3: Testing Error Functions

### Current Code (Hard to Test)

```c
// src/utils/error.c
void custom_error_fd2(char *context, char **args, int exit_status) {
    ft_putstr_fd(context, 2);
    ft_putstr_fd(": command not found\n", 2);
    ft_split_free(args);
    exit(exit_status);  // ← Test process dies here
}
```

**Problem**: Can't test because it calls `exit()`.

### Refactored (Testable)

```c
// src/utils/error_formatting.c (NEW - Pure functions)
/**
 * Format error message (pure function)
 */
char *format_command_not_found_error(const char *cmd_name) {
    size_t len = strlen(cmd_name) + strlen(": command not found\n") + 1;
    char *msg = malloc(len);
    if (!msg)
        return NULL;
    
    strcpy(msg, cmd_name);
    strcat(msg, ": command not found\n");
    return msg;
}

char *format_pipex_error(const char *context, const char *description) {
    size_t len = strlen("pipex: ") + strlen(context) + 
                 strlen(": ") + strlen(description) + strlen("\n") + 1;
    char *msg = malloc(len);
    if (!msg)
        return NULL;
    
    strcpy(msg, "pipex: ");
    strcat(msg, context);
    strcat(msg, ": ");
    strcat(msg, description);
    strcat(msg, "\n");
    return msg;
}
```

```c
// src/utils/error.c (REFACTORED)
void custom_error_fd2(char *context, char **args, int exit_status) {
    char *msg = format_command_not_found_error(context);
    if (msg) {
        ft_putstr_fd(msg, 2);
        free(msg);
    }
    ft_split_free(args);
    exit(exit_status);
}

void error_fd2(char *context, char *description, int exit_status) {
    char *msg = format_pipex_error(context, description);
    if (msg) {
        ft_putstr_fd(msg, 2);
        free(msg);
    }
    exit(exit_status);
}
```

### Unit Tests ✓

```c
// tests/unit/test_error_formatting.c
#include <criterion/criterion.h>
#include "../../include/error_formatting.h"

Test(format_command_not_found_error, basic) {
    char *result = format_command_not_found_error("grep");
    
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "grep: command not found\n");
    
    free(result);
}

Test(format_pipex_error, basic) {
    char *result = format_pipex_error("open", "Permission denied");
    
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "pipex: open: Permission denied\n");
    
    free(result);
}

Test(format_pipex_error, empty_strings) {
    char *result = format_pipex_error("", "");
    
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "pipex: : \n");
    
    free(result);
}
```

---

## Example 4: Integration Test Structure

### Good Integration Test

```bash
# tests/input/10_command_with_spaces.in
infile "grep 'test pattern'" "wc -l" outfile

# tests/input/10_command_with_spaces.exp
42

# tests/input/10_command_with_spaces.status
0
```

### Test Execution
```bash
$ make test-integration
[integration tests] bash tests/test_runner.sh
✅  00_basic
✅  01_wrong_args
✅  02_basiclsls
✅  03_access_second_command_not_found
✅  04_nofile
✅  05_open_no_perms
✅  06_open_wrong_filename
✅  07_execve_first_command_no_perms
✅  07_execve_second_command_no_perms
✅  10_command_with_spaces
```

---

## Example 5: What NOT to Unit Test

### ❌ Don't Unit Test Wrappers Around System Calls

```c
// src/utils/error.c
void xclose(int fd) {
    if (close(fd) == -1)
        fatal_sys("close", 1);
}

pid_t xfork(void) {
    pid_t pid = fork();
    if (pid == -1)
        fatal_sys("fork", 1);
    return pid;
}
```

**Why not?**
- These are thin wrappers
- You'd be testing `close()` and `fork()`, not your code
- Better tested via integration tests

### ✅ Do Integration Test Instead

```bash
# tests/integration/test_fork_failure.sh
# Test pipex behavior when system resources are exhausted

# Simulate fork failure (requires special setup)
ulimit -u 10
./pipex infile "cat" "wc" outfile
status=$?

if [ $status -eq 1 ]; then
    echo "✅ Handles fork failure correctly"
else
    echo "❌ Should exit with status 1 on fork failure"
fi
```

---

## Summary: What to Test Where

| Code | Testing Approach | Tool |
|------|-----------------|------|
| `path_from_cmdname()` | ✅ Unit test | Criterion |
| `get_path_envp()` | ✅ Unit test | Criterion |
| `ft_split_free()` | ✅ Unit test | Criterion |
| `parse_and_resolve_command()` | ✅ Unit test (if refactored) | Criterion |
| `format_error_message()` | ✅ Unit test (if extracted) | Criterion |
| `child1_cmd1()` | ❌ Integration test only | Bash |
| `callexecve1()` | ❌ Integration test only | Bash |
| `xfork()`, `xclose()` | ❌ Integration test only | Bash |
| Full pipex execution | ❌ Integration test | Bash |
| Memory leaks | ❌ E2E test | Valgrind |

## Key Takeaways

1. **Extract pure logic** from functions that call system calls
2. **Test logic separately** from execution
3. **Use integration tests** for multiprocess behavior
4. **Don't force unit tests** where they don't make sense
5. **Refactoring for testability** improves code design

