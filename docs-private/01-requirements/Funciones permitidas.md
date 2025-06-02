# Funciones

## Access

The mode specifies the accessibility check(s) to be performed, and is either the value F_OK, or a mask consisting of the bitwise OR of one or more of R_OK, W_OK, and X_OK. F_OK tests for the existence of the file. R_OK, W_OK, and X_OK test whether the file exists and grants read, write, and execute permissions, respectively.

On success (all requested permissions granted), zero is returned. On error (at least one bit in mode asked for a permission that is denied, or some other error occurred), -1 is returned, and errno is set appropriately.

## dup, dup2

dup2(42, 5)
se lee como que redirigimos 5 a 42 o bien como que a 42 le gustaría ser el 5 por lo que lo duplicamos. 
Así, se entiende mejor que una vez aplicado dup2(42, 5) cerramos 42. 

## Fork

fork(): This function creates a new process by making a copy of the current one. When fork() is called, two processes are created: the parent and the child. The parent process continues its execution from where fork() was called, while the child process starts from the same point but with a different process ID (PID). Additionally, if a pipe was previously created using pipe(), the file descriptors obtained from the pipe are shared between the parent and child processes. This means that data written to one descriptor can be read from the other. fork() essentially duplicates the parent's memory and file descriptors for the child, allowing them to share information and work together.


```c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid;

    pid = 9;
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");    
    printf("Hello1, I am main process (before fork). My PID is %d.\n", getpid());
    // Create a child process using fork()
    pid = fork();
    printf("------------------------------------\n");     
    printf("Hello1, I am main process (after fork). My PID is %d.\n", getpid());
    if (pid < 0) {
        // Fork failed
        fprintf(stderr, "Fork2 failed.\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Hello1, I am the child process! My PID is %d, and my child's PID is %d\n", getpid(), pid);
    } else {
        // Parent process
        printf("Hello1, I am the parent process! My PID is %d, and my child's PID is %d.\n", getpid(), pid);
    }

    return 0;
}
```

## Execve

Everytime I call a command shell calls to execve. 
Lo puedo comprobar con el comando strace. Por ejemplo:
strace "ls"
execve("/usr/bin/ls", ["ls"], 0x7ffca7549190 /* 48 vars */) = 0
brk(NULL)                               = 0x55caf6c5c000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffec5688620) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f0e81a12000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache"
...etc


¿qué parámetros admite?
int execve(const char *__path, char *const __argv[], char *const __envp[])

## Pipe


```c
#include <unistd.h>
#include <stdio.h>

int main()
{
 int  pfd[2];
 char str[20];

 pipe(pfd);
 write(pfd[1], "hello", 5);
 read(pfd[0], str, 2);
 str[2] = '\0';
 printf("%s\n",str);
}
```

Bibliografía: 
https://jan.newmarch.name/OS/l9_1.html
https://people.cs.rutgers.edu/~pxk/416/notes/c-tutorials/pipe.html?source=post_page-----5b4afa4a37b7---------------------------------------


## Sobre wait y waitpid

# Bash Redirection and Pipelines

In Bash, the symbols `<`, `>`, and `|` are used for redirection and pipelines, allowing you to control input and output streams of commands.

## Input Redirection

The `<` symbol is used for input redirection. It allows you to redirect the input of a command from a file. For example:

```bash
$ command < input.txt
```

This will execute `command` with the contents of `input.txt` as its input.

## Output Redirection

The `>` symbol is used for output redirection. It allows you to redirect the output of a command to a file. For example:

```bash
$ command > output.txt
```

This will execute `command` and save its output to `output.txt`.

## Appending Output

If you want to append the output of a command to a file instead of overwriting it, you can use the `>>` symbol. For example:

```bash
$ command >> output.txt
```

This will append the output of `command` to the end of `output.txt`.

## Pipelines

The `|` symbol is used for creating pipelines. It allows you to chain multiple commands together, where the output of one command becomes the input of the next. For example:

```bash
$ command1 | command2
```

This will execute `command1` and pass its output as the input to `command2`.

Pipelines are a powerful feature of Bash, enabling you to combine commands and perform complex operations.

## Examples about pipex < archivo1 comando1 | comando2 > archivo2

< archivo1: This part of the command uses the input redirection operator < to take the contents of archivo1 as input for the subsequent command.

comando1: This is the first command in the pipeline. It will receive the contents of archivo1 as input.

|: The pipe operator | is used to connect the output of comando1 to the input of comando2. It allows the output of one command to be used as the input for another command.

comando2: This is the second command in the pipeline. It will receive the output of comando1 as input.

> archivo2: This part of the command uses the output redirection operator > to redirect the output of comando2 to archivo2. The contents produced by comando2 will be written to archivo2.

```
sort < names.txt
< names.txt sort
< names.txt cat | sort
```
This three commands are equivalent. They all read the contents of names.txt and sort them alphabetically. The output is displayed on the terminal in the first two commands, while the third command redirects the sorted output to a file named names_sorted.txt.

### Key Points
< names.txt redirects the contents of names.txt to standard input that waits until sorts take it
