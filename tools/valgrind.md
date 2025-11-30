Valgrind How-To for pipex
=========================

Goal: Manually verify pipe/file descriptor handling and memory hygiene in `./pipex` while minimizing noise from external commands (e.g. `ls`, `grep`).

1. Build Modes
--------------
- Release (default): `make`
- Address/UB sanitizers (fast feedback): `make fsanitize`
- Debug symbols (better line info in Valgrind): `make debug`

Prefer `make debug` for Valgrind runs; LLVM sanitizers and Valgrind should not be combined (they interfere / slow heavily).

2. Basic Leak / Error Check
---------------------------
```bash
make debug
valgrind --leak-check=full --show-leak-kinds=all \
				 --track-origins=yes --errors-for-leak-kinds=all \
				 ./pipex infile "cat" "wc" outfile
```

3. Following Child Processes
----------------------------
`pipex` forks twice; by default Valgrind reports only the parent. Use:
```bash
valgrind -v --trace-children=yes --child-silent-after-fork=yes \
				 --leak-check=full --show-leak-kinds=all \
				 ./pipex infile "cat" "wc" outfile
```
Notes:
- `--child-silent-after-fork=yes` reduces duplicated startup chatter.
- External commands may legitimately leak (their own libc usage). Focus first on descriptors and memory allocated by pipex prior to `execve`.

4. Open File Descriptor Tracking
--------------------------------
At process exit list remaining FDs owned by pipex:
```bash
valgrind --track-fds=yes ./pipex infile "cat" "wc" outfile
```
Look for unintended still-open copies of pipe ends or the input/output files.

5. Filtering Noise (Suppressions)
---------------------------------
Generate a template suppression file:
```bash
valgrind --tool=memcheck --gen-suppressions=all --log-file=vg.log \
				 ./pipex infile "ls" "wc" outfile
grep 'leaked' vg.log | head -n 20
```
Then craft `valgrind.supp` entries for persistent, third‑party leaks (e.g., inside `ls`). Run with:
```bash
valgrind --suppressions=valgrind.supp --leak-check=full ./pipex ...
```
Keep suppressions minimal; never suppress pipex's own allocations.

6. Per-Process Logs
-------------------
When tracing children, produce distinct logs:
```bash
valgrind --trace-children=yes --log-file=valgrind.%p.log ./pipex infile "cat" "wc" outfile
```
Inspect all `valgrind.*.log` files.

7. Quick FD / Pipe Validation Scenario
-------------------------------------
Test an invalid second command to ensure pipe cleanup:
```bash
valgrind --trace-children=yes --track-fds=yes \
				 ./pipex infile "cat" "nonexistent_cmd" outfile
```
Expect:
- Proper closing of write end of first pipe once exec fails.
- No leaked file descriptors (except standard 0/1/2).

8. Typical Healthy Output Checklist
-----------------------------------
- No "definitely lost" blocks attributed to pipex object files.
- `--track-fds` shows only `stdin`, `stdout`, `stderr` at exit.
- Pipes closed: no lingering entries like `pipe:[XXXX]`.
- When commands fail, still no leaks from error path.

9. Troubleshooting Tips
-----------------------
- If Valgrind output is empty for children, ensure `fork()` then `execve()` paths are actually reached.
- For very short-lived children, append `sleep 0.05` inside debug builds (temporary) before `execve` to allow attaching tools (remove afterward).
- Use `strace -f -e trace=close,open ./pipex ...` alongside Valgrind to corroborate descriptor lifecycle.

10. Minimal Script Example
--------------------------
```bash
#!/usr/bin/env bash
set -euo pipefail
make debug
cmds=("cat" "wc" "ls" "grep hello")
for c1 in "${cmds[@]}"; do
	for c2 in "${cmds[@]}"; do
		valgrind --leak-check=full --trace-children=yes \
						 --log-file=valgrind.$(echo "$c1-$c2" | tr ' ' '_').log \
						 ./pipex tests/e2e/case_cat_wc/bash/infile.txt "$c1" "$c2" /tmp/pipex_out >/dev/null 2>&1 || true
	done
done
grep -H "definitely lost" valgrind.*.log | grep -v " 0 bytes"
```

Focus any fixes on allocation / close sequences inside your own `src/` files, not third‑party command internals.

