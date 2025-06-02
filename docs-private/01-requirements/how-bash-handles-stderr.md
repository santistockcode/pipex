### What’s really happening in

```bash
< infile "ls" | "lsasdfas" > outfile 2> error.log
```

| Phase                                  | Who emits the error?                                                                                                                                                                   | Where is its **stderr** connected?                                                                                          | Message you saw                                                |
| -------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------- |
| **1. Redirection of `< infile`**       | **The *parent* interactive shell** (the one you’re typing in) tries to open `infile` before it even spawns a pipeline process. The `open()` fails with `EACCES`.                       | *The parent shell’s* stderr – **not redirected**, because redirections are applied **after** redirection processing errors. | `bash: infile: Permission denied`  → shows on your terminal    |
| **2. Command lookup for `"lsasdfas"`** | A **sub-shell** that executes the *second* element of the pipeline. It has already inherited the pipeline redirections. When it cannot find the command in `PATH` it prints the error. | The sub-shell’s stderr is the pipeline’s stderr, which is then sent through `2> error.log`.                                 | `bash: lsasdfas: command not found`  → captured in `error.log` |

#### Key points to remember

1. **Redirection errors happen before any command in the pipeline
   starts.** They are produced by your *current* shell process, whose
   stderr is *not* affected by the trailing `2> …` that belongs to the
   pipeline.

2. **`2> error.log` is attached to the pipeline’s stderr only** – i.e.,
   to the subshell(s) that actually execute `ls` and `lsasdfas`.
   Errors these subshells print (like “command not found”) go into the
   file.

3. The parent shell keeps its own stderr unchanged, so its diagnostics
   still appear on the terminal.

#### How Bash decides which stderr is redirected

```
< infile cmd1 | cmd2 > outfile 2> error.log
                └────────────┘   └─────────────┘
             pipeline stdout      pipeline stderr
```

* First Bash parses redirections **inside each simple command** (`<`,
  `>`, pipes).
* Only after building the pipeline does it attach the trailing redirections
  (`> outfile 2> error.log`) to the *pipeline as a whole*.
* Errors that occur **during parsing or redirection setup** belong to the
  parent shell and bypass these pipeline-level redirections.

Keep this mental model and you’ll know exactly where each error message
will land when you test your `pipex`.
