Pensó durante un par de segundos


### `fsanitize` — Why it matters & how to use it

---

#### 1. What the target does

`make fsanitize` rebuilds **pipex** with  

```bash
-fsanitize=address,undefined -g3
```

and links the runtime libraries so that **every run of the resulting binary** is watched by:

| Sanitizer                              | Catches                                                                             | Typical 42‑style bugs it reveals                                                              |
| -------------------------------------- | ----------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- |
| **AddressSanitizer (ASan)**            | heap‑buffer‑overflow, stack‑overflow, use‑after‑free, double‑free, leaks (optional) | off‑by‑one in `ft_strlcpy`, forgetting `+1` for `'\0'`, freeing twice in error paths          |
| **UndefinedBehaviorSanitizer (UBSan)** | signed‑integer‑overflow, divide‑by‑zero, null‑ptr‑deref, shifting by ≥ width        | parsing `atoi` into `int`, unchecked `cmd[i++]=NULL`, bad bit‑shifts when building open flags |

Think of ASan/UBSan as *instant* Valgrind: you run the program once and get a coloured stack trace at the exact defect, with line numbers and variable values.

---

#### 2. How to run it

```bash
make fsanitize
./pipex infile "grep foo" "wc -l" outfile
```

If a bug is detected the program aborts, prints a stack trace and returns **exit code 1** (CI will fail).

You can combine the build with your existing test scripts:

```bash
make fsanitize
make test            # run the same test suite under sanitizers
```

*Tip*: keep separate artefacts (`pipex` vs `pipex_asan`) if you want both release and sanitized binaries side‑by‑side:

```make
fsanitize: NAME := pipex_asan
fsanitize: CFLAGS += $(SAN_FLAGS)
```

---

#### 3. Reading the output (mini‑tour)

```
==11142==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000000018
READ of size 1 at 0x602000000018 thread T0
    #0  ft_strcpy     src/utils/ft_strcpy.c:12
    #1  build_cmd     src/parse/build_cmd.c:45
    #2  main          src/main.c:33
0x602000000018 is located 0 bytes to the right of 8‑byte region [0x602000000010,0x602000000018)
```

* The **first stack frame** (`ft_strcpy`) is usually where the bug originates.
* ASan highlights the precise region (here: just after an 8‑byte buffer).
* UBSan reports the exact expression causing UB, e.g. `signed integer overflow: 2147483647 + 1 cannot be represented`.

---

#### 4. Comparing to Valgrind

| Aspect            | Sanitizers                               | Valgrind                      |
| ----------------- | ---------------------------------------- | ----------------------------- |
| Overhead          | \~2× CPU, +50 % RAM                      | 10–20× CPU, +200 % RAM        |
| Precision         | byte‑level, exact line                   | sometimes only function‑level |
| Leak detection    | optional (`ASAN_OPTIONS=detect_leaks=1`) | always on                     |
| Kernel / syscalls | needs ≥ Linux 3.19, macOS‑Clang works    | works even on old kernels     |
| False positives   | very rare                                | rare                          |
| Production use    | compile‑time flag                        | external tool                 |

*Sanitizers are faster and give clearer traces; Valgrind is still useful for **leak summaries** and double‑checking edge cases that ASan can’t intercept (e.g. file‑descriptor leaks, threading issues).*

---

#### 5. Practical workflow

1. **Write / update code**
2. `make fsanitize`
3. **Run failing test** (or simply start the program)
4. **Fix the first sanitizer crash**
5. **Repeat** until clean
6. Re‑run `make valgrind` to be 100 % sure before you push

In CI we’ll have a dedicated **asan‑ubsan** job that does exactly steps 2–5 on every PR.

---

#### 6. Common tips & gotchas

| Situation                                                                | Solution                                                                                               |
| ------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------ |
| Program aborts in `calloc` before `main`                                 | You linked a debug object with release objects—clean & rebuild.                                        |
| Leak reports are noisy                                                   | `export ASAN_OPTIONS=detect_leaks=0` during integration tests, then enable for the dedicated leak job. |
| Using `system()` spawns `/bin/sh` which inherits ASan; output gets messy | Wrap `system` calls in `__attribute__((no_sanitize("address")))`.                                      |
| macOS: “unable to map pages in ASLR region”                              | `export MallocNanoZone=0` before running the binary.                                                   |

---

#### 7. When to turn it off

* Performance benchmarks (it slows I/O by \~30 %).
* When a known third‑party bug spams the log (rare in 42 projects).
* Final **release** artefact shipped to the evaluator should be built with `make all`, not `fsanitize`.

---

**Bottom line:** `fsanitize` is the fastest way to catch 90 % of memory & UB errors *early*—before you even start Valgrind or push to GitHub. Treat a sanitizer crash as a unit‑test failure: fix immediately, commit only when clean.
