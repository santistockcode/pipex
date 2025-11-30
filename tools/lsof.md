Using lsof for pipex
====================

Goal: Detect unintended open file descriptors (especially pipe ends or input/output files) remaining in parent or children after setup/fork.

1. Basic Invocation Pattern
---------------------------
`pipex` exits quickly; to observe FDs, pause execution before `execve` or use a long‑running command (e.g. `sleep 5`).
```bash
./pipex infile "cat" "sleep 5" outfile & pid=$!
sleep 0.2
lsof -p $pid
wait $pid
```
Look for duplicated `infile`, `outfile`, or extra `pipe:[NNNNNN]` instances.

2. Interpreting Output
----------------------
- FD column: numbers (0,1,2 std), plus others (e.g. 3,4 for pipe ends).
- TYPE `FIFO` or `pipe` indicates pipe ends; they should exist only until exec replaces the child.
- `DEL` or `(deleted)` entries usually not relevant unless you unlink temp files prematurely.

3. Parent vs Children
---------------------
Trace both processes when forking:
```bash
./pipex infile "sleep 3" "sleep 3" outfile & p=$!
ps --ppid $p -o pid= | xargs -r -I{} lsof -p {}
```
If the parent keeps pipe FDs open after spawning both children, they may not receive EOF properly.

4. Using /proc Directly (Alternative)
-------------------------------------
```bash
ls -l /proc/$pid/fd
readlink /proc/$pid/fd/3
```
This is faster than `lsof` for quick checks.

5. GDB Breakpoint Strategy
---------------------------
Start under gdb and break just after pipe/dup setup but before `execve`:
```bash
gdb --args ./pipex infile "cat" "wc" outfile
(gdb) break child1_cmd1.c:42   # example line after dup2
(gdb) run
(gdb) ! lsof -p $(pidof pipex)
```
Replace file/line with the actual spot right after descriptor configuration.

6. EOF / Closure Test
---------------------
If you intentionally close write ends early to signal EOF, verify only read ends remain:
```bash
./pipex infile "cat" "wc" outfile & pid=$!
sleep 0.1
lsof -p $pid | grep pipe
```
Unexpected lingering write ends can cause `wc` to hang waiting for more data.

7. Detecting Leaks Across Many Runs
-----------------------------------
Loop and aggregate FD counts (should stay stable):
```bash
for i in {1..50}; do
	./pipex infile "cat" "wc" outfile >/dev/null 2>&1 & p=$!
	sleep 0.05
	lsof -p $p | awk 'NR>1{c++} END{print c}' >> fd_counts.txt || true
done
sort -n fd_counts.txt | uniq -c
```
Large variance may indicate occasional failure to close descriptors in error paths.

8. Common Issues & Fixes
------------------------
- Multiple open copies of the same pipe end: ensure you `close()` after `dup2()`.
- Output file descriptor left open in parent after children finish: close after fork if parent no longer needs it.
- Hanging second command: usually due to parent retaining a write end of the first pipe.

9. Combining with strace
------------------------
```bash
strace -f -e trace=pipe,dup,close,open ./pipex infile "cat" "wc" outfile 2>&1 | less
```
Correlate each `open/pipe/dup/close` with lsof snapshots to confirm lifecycle.

10. Minimal Sleep Wrapper for Observation
-----------------------------------------
When real commands exit instantly, wrap them:
```bash
sh -c 'cat; sleep 2'   # keeps FD open briefly
sh -c 'wc; sleep 2'
```
Use these in place of fast commands to inspect state mid‑execution.

Capture anomalies early; fix descriptor hygiene before deep integration tests.

