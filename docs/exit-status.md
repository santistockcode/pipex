### What `echo $?` really shows

`$?` is the **exit status** (0-255) of the very last program the shell
ran.
`0` means “success”; any non-zero value is an error *code* chosen by that
program.

```bash
true    ; echo $?   # → 0   (success)
false   ; echo $?   # → 1   (generic failure)
ls zzzz ; echo $?   # → 2   (ls could not stat file)
```

---

## How shells pick exit codes for *commands that never even ran*

When you type an external command the shell does two steps:

1. **Search** `PATH` for an executable file.
2. **execve()** it.

If either step fails, the shell invents a *conventional* exit status:

| Situation                                                | `errno`             | Shell exit code |
| -------------------------------------------------------- | ------------------- | --------------- |
| **command not found** (no file in `PATH`)                | `ENOENT`            | **127**         |
| **found but not executable** (permission, binary format) | `EACCES`, `ENOEXEC` | **126**         |

Bash, zsh, Dash, BusyBox `sh`—they all follow this POSIX convention.

---

## Exit code of a **pipeline**

For `cmd1 \| cmd2` the shell *waits* for each child.
By default, `$?` becomes the exit status of the **last command in the
pipeline** (`cmd2`).
(Bash has `set -o pipefail` to OR-together failures, but POSIX sh doesn’t.)

Example:

```bash
grep foo file.txt | wc -l
echo $?   # exit of wc (0 if wc ran fine, even if grep failed)
```

---

## What this means for **pipex**

Your project must mimic:

```bash
< infile cmd1 | cmd2 > outfile
```

So:

1. If `execve(cmd1)` fails, that child **exits 127** or **126**
   *but the parent keeps running cmd2*.
2. `pipex` itself should finally exit with whatever exit status **cmd2**
   returned, because that’s what a real shell does.

### Minimal C sketch

```c
pid_t c1 = fork();
if (c1 == 0) {                      // first child
    execve(path1, argv1, envp);
    perror(path1);                  // only if execve fails
    exit(errno == ENOENT ? 127 : 126);
}

pid_t c2 = fork();
if (c2 == 0) {                      // second child
    execve(path2, argv2, envp);
    perror(path2);
    exit(errno == ENOENT ? 127 : 126);
}

/* parent */
int st;
waitpid(c2, &st, 0);    // wait for last command
if (WIFEXITED(st))
    exit(WEXITSTATUS(st));
else
    exit(1);            // crashed or signalled
```

(Tip: also `waitpid(c1, NULL, 0);` so you don’t leak zombies.)

---

## Quick checks in your test script

```bash
./pipex infile "badcmd" "wc -l" out;  echo $?  # → 0 (wc ran)
./pipex infile "grep foo" "badcmd"   out; echo $?  # → 127 or 126
```

Compare that with the real shell pipeline to see if `pipex` matches.

---

### TL;DR

* `$?` reports the exit status of the very last command the shell ran.
* Shell uses **127 / 126** to signal “command not found / not executable”.
* `pipex` should propagate the exit status of **its second command**, and
  each failed `execve` must exit with 127 or 126 so peers can diagnose
  problems exactly like they would in Bash.
