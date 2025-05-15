**Explicando las pruebas y herramientas**

El usuario quiere saber sobre algunas herramientas de pruebas como fuzz, valgrind, y las diferencias entre varios scripts. Necesito detallar dos capas de pruebas: las pruebas unitarias con Criterion y las de integración con el script de bash. También explicaré el propósito de cada script en el conjunto de pruebas: el script test\_runner.sh, para pruebas de integración, y valgrind\_suite.sh para pruebas de memoria. Todo debe estar en el contexto de cómo interactúan con los objetivos de Make.


Pensó durante 6 segundos


## “Test”, “Valgrind”, and “Fuzz” — how they fit together

Below is the mental model I use on 42 projects; once you see the **four layers** the Makefile targets & scripts will make sense.

| Layer                       | Goal                                                           | Tooling                                                      | Triggered by                              |
| --------------------------- | -------------------------------------------------------------- | ------------------------------------------------------------ | ----------------------------------------- |
| **1. Unit**                 | Prove each *function* behaves                                  | **Criterion** (or simple `assert`) <br>one *test binary*     | `make test‑unit` *(inside* `make test`)\* |
| **2. Integration / golden** | Prove the whole *program* gives correct output for fixed cases | Bash **`test_runner.sh`** (diffs `stdout`, `stderr`, `exit`) | `make test`                               |
| **3. Memory**               | Same integration cases but leak‑checked                        | Bash **`valgrind_suite.sh`** (grep “ERROR SUMMARY: 0”)       | `make valgrind`                           |
| **4. Property / fuzz**      | Throw *random* inputs at the parser to find crashes            | **libFuzzer** harness (`fuzz‑run` target)                    | `make fuzz‑run` (30 s)                    |

> **Mnemonic:**  **U**nit → **G**olden → **M**emory → **F**uzz
> *(UGMF—run them in that order while coding).*

---

### 1. Unit tests (Criterion)

```
tests/unit/
├── test_is_alpha.c
└── test_split.c
```

```c
// tests/unit/test_is_alpha.c
#include <criterion/criterion.h>
#include "libft.h"

Test(ft_isalpha, lowercase_letters) {
    for (char c = 'a'; c <= 'z'; ++c)
        cr_expect(eq(int, ft_isalpha(c), 1));
}
```

*Compiling*: You build a **separate binary** (`unit_tests`) that links your `.o` files *and* Criterion.
The `make test` rule can simply do:

```make
test-unit:
	$(CC) $(CFLAGS) $(addprefix tests/unit/, *.c) $(OBJ) -lcriterion -o unit_tests
	./unit_tests --fail-fast
```

Criterion prints coloured output and returns non‑zero if anything fails.

---

### 2. Integration tests (`test_runner.sh`)

Folder layout:

```
tests/golden/
├── 00_basic.in
├── 00_basic.exp
├── 01_here_doc.in
└── 01_here_doc.exp
```

**`tools/test_runner.sh`** (very small):

```bash
#!/usr/bin/env bash
bin="$1"        # ./pipex
indir="$2"      # tests/golden
outdir="$3"     # tests/tmp

fail=0
for in_file in "$indir"/*.in; do
    base=$(basename "$in_file" .in)
    exp="$indir/$base.exp"
    out="$outdir/$base.out"

    $bin $(cat "$in_file") >"$out" 2>&1
    if diff -u "$exp" "$out"; then
        printf '✅  %s\n' "$base"
    else
        printf '❌  %s\n' "$base"; fail=1
    fi
done
exit $fail
```

`make test` calls that script **after** running the unit suite:

```make
test: test-unit
	$(Q)bash tools/test_runner.sh $(PIPEX_BIN) tests/golden tests/tmp
```

---

### 3. Valgrind suite (`valgrind_suite.sh`)

Same idea, but wrapped in Valgrind:

```bash
#!/usr/bin/env bash
bin="$1"
cases=( "tests/golden/"*.in )
for in_file in "${cases[@]}"; do
    args=$(cat "$in_file")
    log="tests/valgrind/$(basename "$in_file" .in).log"

    valgrind --leak-check=full --error-exitcode=42 \
             --log-file="$log" $bin $args || exit 1

    # (Valgrind already exits 42 on error, so reaching here means PASS)
    printf '✅  %s\n' "$in_file"
done
```

The Make rule:

```make
valgrind: all
	$(Q)bash tools/valgrind_suite.sh "$(PIPEX_BIN)"
```

CI fails if **any** run returns exit 42.

---

### 4. Fuzzer harness (`fuzz-run`)

1. **Add** a tiny file `tests/fuzz/harness.c`:

```c
#ifdef FUZZING
#include <stdint.h>
#include <stddef.h>
#include "pipex.h"   /* or whatever parses argv */

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* turn the random blob into fake argv */
    char *tmp = strndup((const char *)data, size);
    char *argv[] = { "pipex", tmp, NULL };
    main(2, argv);           /* ignore return */
    free(tmp);
    return 0;
}
#endif
```

2. The `fuzz` Make target already sets:

```make
CFLAGS += -fsanitize=fuzzer,address -DFUZZING
```

3. Build & run:

```bash
make fuzz
./pipex          # libFuzzer auto‑runs; Ctrl‑C after ±30 s
```

4. Crashes appear with full ASan trace; the corpus in `tests/fuzz/corpus/` evolves automatically (commit it if you like).

---

### Putting it all together in CI

```yaml
# .github/workflows/build.yml  (snippet)
- name: Unit + golden tests
  run: make test

- name: ASan/UBSan
  run: make fsanitize && make test

- name: Valgrind
  run: make valgrind

- name: Fuzz smoke‑test
  run: make fuzz-run || true   # allow job to continue after 60 s
```

---

## TL;DR

* **`make test`**

  * compiles & runs *unit tests* (Criterion)
  * runs *golden* bash script to diff outputs.

* **`make valgrind`**

  * re‑runs the same golden cases **under Valgrind**; fails on any leak/error.

* **`make fuzz-run`**

  * builds special *instrumented* binary and lets libFuzzer hammer your parser for 30 seconds; you watch for crashes.

Each layer catches a different class of bug, and you keep them separated so that:

* Unit tests stay lightning‑fast.
* Valgrind overhead doesn’t slow every test run.
* Fuzzing can be optional/offline yet still wired into Make.

Tell me which script you’d like written first (or copy the skeletons above and we’ll iterate), and we’ll flesh it out.
