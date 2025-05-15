Tips & Improvements

    Wrap all syscalls (xopen, xpipe, xfork) so 90 % of error handling lives in one C file.

    One global struct (t_px) holding pipes array, pids, envp ⇒ simplifies cleanup.

    Free list of strings with one helper ft_free_split(char **) you’ll reuse in Minishell.

    Keep libft unmodified; add pipex-specific utilities separately to avoid Norminette surprises.

    Git branches: create one per phase; merge only when leak-free and Norm-clean.

    Future-proof: write exec_cmd(char *cmd, char **envp) now—later you’ll swap parsing to Minishell without touching exec logic.