# 42 ⟫ pipex ― *a tiny shell pipeline in C*

![grade](https://img.shields.io/badge/42%20eval-100%25-brightgreen?style=flat-square)


> **TL;DR**  
> Re-implement  
> ```sh
> < file1  cmd1  |  cmd2  > file2
> ```  
> in **124 ~ lines of pure C**, with full error-handling, exit-status
> parity, Valgrind-clean results and no forbidden libc calls.

---

## 👋 Welcome — read this first!

| If you want… | …then check |
|--------------|-------------|
| the **exact version** that scored **100 %** at 42 Madrid (May 2025) | **`main`** branch (/include, /libft, /src folders + Makefile) |
| my ongoing refactor + bonus + try to understand minishell | **[`develop`](https://github.com/saalarco/pipex/tree/develop)** branch |

> **Why two branches?**  
> *main* is frozen for archival/reference.  
> *develop* is where I continue developing this project (bonus are needed for minishell)

To clone and jump straight to the good stuff:

```bash
git clone https://github.com/saalarco/pipex.git
cd pipex
git switch develop        # ← optional
