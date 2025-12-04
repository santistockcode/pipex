## Error handling vs manual testing

Error handling appears when a pipeline or something like it is involved, from the moment multiple modules are involved we need error handling. No los confundo.

## lsof
(ver lsof.md)

## valgrind
(ver valgrind.md)

## The ones that appear in manual-tests.png
(see manual-tests.png)

## Sleep and Timing Scenarios

Pipelines with `sleep` are useful to validate blocking and ordering:

- `"grep hello" | "sleep 1" | "wc -l"` → Output likely empty; ensure final status matches bash.
- Verify parent waits for the last command (`waitpid`) and does not deadlock when intermediate stages produce no output.

Run with debug logging to trace behavior:

```bash
make debug
./pipex input "grep hello" "sleep 1" "wc -l" outfile
```

You’ll see `[pipex][file:line]` logs for pipe creation and command spawning.

## ls report error, is valgrind ok?

```bash
dev@73d0c30eceaf:/workspace$ ./pipex infile wc ls outfile
dev@73d0c30eceaf:/workspace$ exec 1<&-
dev@73d0c30eceaf:/workspace$ ./pipex infile wc ls outfile
ls: write error: Bad file descriptor
dev@73d0c30eceaf:/workspace$ ./pipex infile wc ls outfile >&2
```

## Test that on open fail cmd does not run

1. create a command that announces itself
echo 'echo "I WAS RUN" >> /tmp/cmd_log' > /tmp/mycmd.sh
chmod +x /tmp/mycmd.sh

2. make sure log is clean
rm -f /tmp/cmd_log

3. case 1: missing infile, with pipeline
./tmp/mycmd.sh < no_such_file | wc -l

4. check log
cat /tmp/cmd_log

## Test that here_doc prevails over pipe (note, in zsh doesn't work like this)
```bash
bash-3.2$ printf "A\nB\nC\n" | cat | cat << EOF | wc -l
> line1
> line2
> line3
> line4
> EOF
       4
bash-3.2$
```
