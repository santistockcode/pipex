## 1. Execve will fail when no shebangs

1. create a command that announces itself
echo 'echo "I WAS RUN" >> /tmp/cmd_log' > /tmp/mycmd.sh
chmod +x /tmp/mycmd.sh

2. make sure log is clean
rm -f /tmp/cmd_log

3. case 1: missing infile, with pipeline
./tmp/mycmd.sh < no_such_file | wc -l

4. check log
cat /tmp/cmd_log

If I use a REAL infile, execve will still fail. Bash would manage ENOEXEC and use sh to execute it. A workaround is calling shell, but is not required: 

```c
if (execve(path, args, envp) == -1)
{
    if (errno == ENOEXEC)
    {
        // Run script via /bin/sh
        char *sh_argv[64]; // or dynamic
        size_t i = 0;

        sh_argv[i++] = "/bin/sh";
        sh_argv[i++] = (char *)path;
        for (size_t j = 1; args[j] && i < 63; ++j)
            sh_argv[i++] = args[j];
        sh_argv[i] = NULL;

        execve("/bin/sh", sh_argv, envp);
        // if this fails, fall through to generic error
    }
    pipex_handle_execve_error(args, path, "execve");
}
```