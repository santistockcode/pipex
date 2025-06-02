/*
Exercise 4: Using pipe for Inter-Process Communication

    Goal: Learn pipe and its use with fork.

    Write a C program that:

        Creates a pipe using pipe.

        Forks a child process.

        In the parent process, write a message to the pipe.

        In the child process, read the message from the pipe and print it.
        */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main(void)
{
    int fd[2];
    pid_t pid;
    char message[] = "Hello from parent!";

    if (pipe(fd) < 0)
    {
        perror("pipe failed");
        return 1;
    }

    pid = fork();

    if (pid < 0)
    {
        // Error
        perror("fork failed in parent process");
        return 1;
    }
    else if (pid == 0)
    {
        printf("Child process\n");
        // Child process
        char buffer[100];

        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        printf("Message from parent: %s\n", buffer);
        close(fd[0]);
    }
    else
    {
        printf("Parent process\n");
        // Parent process
        close(fd[0]);
        write(fd[1], message, sizeof(message));
        close(fd[1]);
        wait(NULL);
    }
    printf("Process finished\n");
    return 0;
}

/*
EXPLANATION 

When a process calls fork(), it creates a new child process that runs concurrently with the parent process. The operating system's scheduler decides which process (parent or child) gets CPU time first. This decision is based on various factors such as process priority, CPU load, and other system-specific criteria.

Because of this non-deterministic scheduling, either the parent or the child process may execute first. This means that the output order of "Parent process" and "Child process" is not guaranteed and can vary between runs.

Here is a detailed explanation of the code execution:

Pipe Creation: The pipe(fd) call creates a pipe, which is a unidirectional data channel that can be used for inter-process communication. The fd array contains two file descriptors: fd[0] for reading and fd[1] for writing.

Forking: The fork() call creates a new process. The parent process receives the child's PID (process ID), while the child process receives 0.

Parent Process:

If pid > 0, the parent process executes this block.
It prints "Parent process".
It closes the read end of the pipe (fd[0]).
It writes the message "Hello from parent!" to the write end of the pipe (fd[1]).
It closes the write end of the pipe (fd[1]).
It waits for the child process to finish using wait(NULL).
Child Process:

If pid == 0, the child process executes this block.
It prints "Child process".
It closes the write end of the pipe (fd[1]).
It reads the message from the read end of the pipe (fd[0]) into the buffer.
It prints the message received from the parent.
It closes the read end of the pipe (fd[0]).
Given the non-deterministic nature of process scheduling, the following scenarios are possible:

Scenario 1: The parent process runs first:

Scenario 2: The child process runs first:

In both scenarios, the message "Message from parent: Hello from parent!" will always be printed after both "Parent process" and "Child process" because the child process reads the message written by the parent. However, the order of "Parent process" and "Child process" can vary due to the operating system's scheduling.
*/