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