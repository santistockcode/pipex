Pensó durante unos pocos segundos


Below is a **mini‑TDD walkthrough** that starts from a **red test**, drives a tiny implementation, and ends with a **green suite** for a helper that resolves a command’s full path by consulting `envp`.

---

## 0 . Folder prep

```
tests/
└── unit/
    ├── test_cmdpath.c      # new – Criterion specs
src/
└── util/
    └── cmdpath.c          # implementation we’ll write
include/
└── util.h                 # public prototype
```

Add the new objects to `SRC` in your Makefile:

```make
SRC  := ... \
        src/util/cmdpath.c
```

---

\## 1 . Choose the API

```c
/* include/util.h */
#ifndef UTIL_H
# define UTIL_H

/* Returns malloc‑ed string with full path or NULL if not found */
char *cmd_resolve_path(const char *cmd, char *const envp[]);

#endif
```

\* `cmd` – just “ls” or “grep”.
\* It must work even if `envp` is `NULL` or PATH missing.
\* Caller frees the returned string.

---

\## 2 . Write the first failing test

```c
/* tests/unit/test_cmdpath.c */
#include <criterion/criterion.h>
#include "util.h"
#include <unistd.h>     // access

Test(cmd_resolve_path, finds_ls_on_normal_path)
{
    char *fake_env[] = { "PATH=/bin:/usr/bin", NULL };
    char *p = cmd_resolve_path("ls", fake_env);
    cr_expect_not_null(p);
    cr_expect(eq(int, access(p, X_OK), 0));   // really executable
    free(p);
}
```

Compile / run:

```bash
make test      # expects failure – undefined reference
```

---

\## 3 . Implement the minimal code

```c
/* src/util/cmdpath.c */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

static char *join_path(const char *dir, const char *cmd)
{
    size_t len = strlen(dir) + 1 + strlen(cmd) + 1;
    char *full = malloc(len);
    if (!full) return NULL;
    strcpy(full, dir);
    strcat(full, "/");
    strcat(full, cmd);
    return full;
}

char *cmd_resolve_path(const char *cmd, char *const envp[])
{
    if (!cmd || strchr(cmd, '/'))
        return strdup(cmd);               /* already a path or NULL */

    /* Locate PATH in envp (envp may be main’s env or a fake list) */
    const char *path_var = NULL;
    if (envp)
        for (int i = 0; envp[i]; ++i)
            if (strncmp(envp[i], "PATH=", 5) == 0)
                { path_var = envp[i] + 5; break; }

    if (!path_var)                         /* fallback: use system getenv */
        path_var = getenv("PATH");
    if (!path_var)                         /* still nothing */
        return NULL;

    char *dup = strdup(path_var);          /* strtok needs modifiable buf */
    char *save = dup;
    for (char *dir = strtok(dup, ":"); dir; dir = strtok(NULL, ":"))
    {
        char *full = join_path(dir, cmd);
        if (!full) break;
        if (access(full, X_OK) == 0)
            { free(save); return full; }
        free(full);
    }
    free(save);
    return NULL;
}
```

Run tests again → **green**.

---

\## 4 . Add more specs (Red → Green loop)

```c
Test(cmd_resolve_path, returns_null_when_not_found)
{
    char *fake_env[] = { "PATH=/tmp", NULL };
    cr_expect_null(cmd_resolve_path("definitely_missing123", fake_env));
}

Test(cmd_resolve_path, handles_missing_path_variable)
{
    char *fake_env[] = { "USER=foo", NULL };
    /* Might fall back to system PATH; isolate with empty getenv */
    setenv("PATH", "", 1);
    cr_expect_null(cmd_resolve_path("ls", fake_env));
}
```

If one fails, adjust implementation.  Example fix: check for empty `dir`.

---

\## 5 . Integrate into **child1\_cmd1**

```c
#include "util.h"

void callexecve1(char *const envp[])
{
    char *path = cmd_resolve_path("ls", envp);
    if (!path)
        exit(ft_printf("command not found\n"), 127);
    char *const args[] = { "ls", NULL };
    execve(path, args, envp);
    perror("execve");
    free(path);
    exit(EXIT_FAILURE);
}
```

Later you will parse `argv[2]` to split command + args; the same helper still applies.

---

\## 6 . Update Makefile test rule (Criterion)

```make
UNIT_SRCS  := $(wildcard tests/unit/*.c)
UNIT_BIN   := unit_tests

test-unit: $(OBJ) $(UNIT_SRCS)
	$(CC) $(CFLAGS) $^ -lcriterion -o $(UNIT_BIN)
	./$(UNIT_BIN) --color --fail-fast
```

Make `test` depend on `test-unit` first, then call the golden script.

---

\## 7 . Run the whole pipeline

```bash
$ make test          # criterion + golden diff
$ make fsanitize && ./pipex infile "ls -l" "wc -l" outfile
$ make valgrind
$ make fuzz-run
```

All layers should now pass with the real‐world function in place.

---

### What you just practised   🌱

1. **Red test** first with Criterion.
2. Implement *just enough* to go green.
3. Add edge‑case specs → refine code.
4. Wire the helper into production flow.

Repeat this cycle section‑by‑section—argument parsing, here‑doc support, error strings—and you’ll steadily grow a fully tested **pipex**.  Whenever you’re ready for the next helper (e.g. splitting quoted arguments) or for writing the golden diff script in detail, let me know!
