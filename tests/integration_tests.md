# Pipex Integration Testing and Failure Injection

see syswrap.md

## Environment Controls (No code changes needed)

- `PIPEX_FAIL_PIPE_AT=N`
  - Fails the Nth call to `pipe()` with `errno=EMFILE` (Too many open file descriptors).
  - Example: fail at the 4th pipe creation.
    ```bash
    PIPEX_FAIL_PIPE_AT=4 ./pipex input "cat" "grep hello" "wc -l" "cat" "wc -c" outfile
    ```

- `PIPEX_FAIL_FORK=1`
  - Makes `fork()` fail with `errno=EAGAIN`.
  - Example:
    ```bash
    PIPEX_FAIL_FORK=1 ./pipex input "cat" "wc" outfile
    ```

- `PIPEX_FAIL_DUP2_AT=N`
  - Fails the Nth `dup2()` call with `errno=EBADF`.
  - Example:
    ```bash
    PIPEX_FAIL_DUP2_AT=2 ./pipex input "cat" "wc" outfile
    ```

- `PIPEX_FAIL_OPEN_PATH=substr`
  - Fails `open()` when the path contains `substr` with `errno=EACCES`.
  - Example (fail opening `outfile.txt`):
    ```bash
    PIPEX_FAIL_OPEN_PATH=outfile ./pipex input "cat" "wc" outfile.txt
    ```

- `PIPEX_FAIL_EXECVE=1`
  - Forces `execve()` to fail with `errno=EACCES`. Exit handling follows shell conventions (126/127) via `pipex_handle_execve_error`.
  - Example:
    ```bash
    PIPEX_FAIL_EXECVE=1 ./pipex input "ls" "wc" outfile
    ```
## Manual Failure Reproduction Scenarios

### Manual Reproduction of EMFILE (Too Many FDs)

To observe real EMFILE behavior without injection, lower the per-process FD limit and open many files:

```bash
# Temporarily lower the soft fd limit (Bash)
ulimit -n 32

# Create lots of descriptors in a shell loop
for i in $(seq 1 40); do exec {fd}<> /dev/null || echo "open failed at i=$i"; done

# Now run your pipeline; pipe() may fail with EMFILE
./pipex input "cat" "grep hello" "wc -l" outfile
```

Restore the limit by exiting bash and attaching again to container.

Example that it works: 
```bash
dev@73d0c30eceaf:/workspace$ ulimit -n 6
dev@73d0c30eceaf:/workspace$ ls -l /proc/$$/fd
bash: start_pipeline: pgrp pipe: Too many open files
total 0
lrwx------ 1 dev dev 64 Dec  1 11:27 0 -> /dev/pts/1
lrwx------ 1 dev dev 64 Dec  1 11:27 1 -> /dev/pts/1
lrwx------ 1 dev dev 64 Dec  1 11:27 2 -> /dev/pts/1
lrwx------ 1 dev dev 64 Dec  1 11:27 255 -> /dev/pts/1
lrwx------ 1 dev dev 64 Dec  1 11:28 3 -> /dev/null
lrwx------ 1 dev dev 64 Dec  1 11:28 4 -> /dev/null
dev@73d0c30eceaf:/workspace$ ./pipex infile ls wc outfile
bash: start_pipeline: pgrp pipe: Too many open files
pipex: pipe: Too many open files
dev@73d0c30eceaf:/workspace$ < infile ls | wc > outfile
bash: pipe error: Too many open files
bash: start_pipeline: pgrp pipe: Too many open files
dev@73d0c30eceaf:/workspace$
```

Practical ways to demonstrate each failure class without relying solely on the injection environment variables. Some (ENFILE, EFAULT, ENOMEM, EINTR) are system‑wide or timing dependent; prefer injection for deterministic CI.

### pipe()
Failure modes: EMFILE (per‑process FD limit) (ver arriba), ENFILE (system‑wide limit), EFAULT (invalid pointer).
- EMFILE:
  ```bash
  ulimit -n 32
  for i in $(seq 1 40); do exec {fd}<> /dev/null || echo "open failed at i=$i"; done
  ./pipex input "cat" "grep hello" "wc -l" outfile
  ```
  Expect: failure path engages `fatal_ctx("pipe")`.
- ENFILE: Hard to cause safely; needs global exhaustion (many processes). Use `PIPEX_FAIL_PIPE_AT` instead.
- EFAULT: Requires deliberate invalid pointer in a custom harness; use injection instead.
Injection quick checks:
```bash
PIPEX_FAIL_PIPE_AT=2 ./pipex input "cat" "wc" outfile
PIPEX_FAIL_PIPE_AT=4 ./pipex input "cat" "grep h" "wc -l" "cat" outfile
```

### fork()
Failure modes: EAGAIN (process quota), ENOMEM.
- EAGAIN:
  ```bash
  ulimit -u 128
  for i in $(seq 1 200); do ( sleep 30 ) & done
  ./pipex input "cat" "wc" outfile
  kill $(jobs -p)
  ```
- ENOMEM: Use stress tools (`stress-ng`) only on sandbox systems; prefer `PIPEX_FAIL_FORK=1`.
Injection:
```bash
PIPEX_FAIL_FORK=1 ./pipex input "cat" "wc" outfile
```

Note on this one: Running a pipeline or any external command requires the shell itself to fork first. When ulimit -u is exhausted, Bash can’t fork a child to exec your pipeline or to start pipex, so the error originates in Bash: “fork: retry: Resource temporarily unavailable”. No lo puedo reproducir, dependo de los tests de integración.

### dup2()
Failure modes: EBADF, EINTR.
- EBADF manual harness example:
  ```c
  int fd=open("/dev/null",O_RDONLY); close(fd); dup2(fd,1); // EBADF
  ```
- EINTR non‑deterministic (signals during heavy looping); rely on injection.
Injection:
```bash
PIPEX_FAIL_DUP2_AT=3 ./pipex input "cat" "grep h" "wc -l" outfile
```

Note on this one: However, with stdout closed in the environment, some tools (including coreutils) may perform early stream setup around stdout before your dup2 runs or may detect invalid stream state; when those internal checks trip, they emit an error to stderr and either avoid writing or exit early. The net effect is the same: you see the error and no content in outfile.

De ahí que si cierro 1, me encuentro con esto:
```bash
dev@73d0c30eceaf:/workspace$ ./pipex infile wc ls outfile
dev@73d0c30eceaf:/workspace$ exec 1<&-
dev@73d0c30eceaf:/workspace$ ./pipex infile wc ls outfile
ls: write error: Bad file descriptor
dev@73d0c30eceaf:/workspace$ ./pipex infile wc ls outfile >&2
// the file is empty
```
Para rearmar stdout corro: 
exec 1>/dev/pts/1
Ls hace alguna comprobación con fd 1 (incluso aunque no vaya a escribir nada ahí porque debe ir a outfile) y reporta un error.

### open()
Failure modes: ENOENT, EACCES.
- ENOENT infile:
  ```bash
  ./pipex missing_file "cat" "wc" outfile
  ```
- EACCES:
  ```bash
  touch locked.txt; chmod 000 locked.txt
  ./pipex locked.txt "cat" "wc" out.txt
  touch out.txt; chmod 000 out.txt
  ./pipex input "cat" "wc" out.txt
  chmod 644 locked.txt out.txt
  ```
Injection:
```bash
PIPEX_FAIL_OPEN_PATH=out.txt ./pipex input "cat" "wc" out.txt
```

### execve()
Failure modes: ENOENT (127), EACCES (126), malformed binary (126).
- ENOENT:
  ```bash
  ./pipex input "no_such_cmd" "wc" out.txt; echo $?
  ```
- EACCES:
  ```bash
  echo '#!/bin/sh\necho hi' > script.sh; chmod 644 script.sh
  ./pipex input "./script.sh" "wc" out.txt; echo $?
  ```
- Bad ELF:
  ```bash
  cp /bin/ls badls; truncate -s 16 badls; chmod +x badls
  ./pipex input "./badls" "wc" out.txt; echo $?
  ```
Injection:
```bash
PIPEX_FAIL_EXECVE=1 ./pipex input "ls" "wc" out.txt
```

### Sleep / Timing
Check delayed/no-output stages:
```bash
./pipex input "sleep 1" "wc -l" out.txt
./pipex input "grep hello" "sleep 1" "wc -l" out.txt
```
Confirm parent waits for last command only.

### Quick Manual Test Matrix
| Category | Manual Scenario | Injection Var | Expected |
|----------|-----------------|---------------|----------|
| pipe     | ulimit -n & many opens | PIPEX_FAIL_PIPE_AT=2 | abort + freed ctx |
| fork     | spawn > ulimit -u       | PIPEX_FAIL_FORK=1    | fatal_ctx("fork") |
| dup2     | closed fd dup2          | PIPEX_FAIL_DUP2_AT=3 | child abort |
| open     | chmod 000 outfile       | PIPEX_FAIL_OPEN_PATH | fatal_ctx("open") |
| execve   | missing command         | PIPEX_FAIL_EXECVE    | exit 127/126 |
| timing   | sleep mid pipeline      | (n/a)                | proper wait |

### Cleanup Verification
List open descriptors (Linux):
```bash
ls -l /proc/$(pgrep -n pipex)/fd | head
```
Sanitizer build:
```bash
make fsanitize
PIPEX_FAIL_PIPE_AT=2 ./pipex input "cat" "wc" out.txt
```

## Notes

- Child error paths only free child-local allocations (e.g., argv arrays, resolved paths). The parent owns and frees the global `t_pipex_ctx`.
- `fatal_ctx` is used on parent-side unrecoverable failures to free context then exit.
- Use `safe_close` for cleanup paths to avoid crashing on invalid or already-closed descriptors.