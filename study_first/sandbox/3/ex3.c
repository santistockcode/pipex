/*
Exercise 3: Creating and Managing Processes

    Goal: Learn fork, wait, and waitpid.

    Write a C program that:

        Creates a child process using fork.

        In the child process, print "Hello from child!" and exit.

        In the parent process, wait for the child to finish using wait or waitpid.

        Print "Child process finished" in the parent process.
        */

#include <stdio.h>
#include <unistd.h>
# include <sys/types.h>

int main(void)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0)
    {
        // Error
        perror("fork failed in parent process"); // I know I'm in parent because there is no child (it failed)
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        printf("Hello from child!\n");
        return 0;
    }
    else
    {
        // Parent process https://www.geeksforgeeks.org/wait-system-call-c/
        if (wait(&status) < 0)
        {
            perror("wait failed in parent process");
            return 1;
        }
        printf("Child process finished\n");
    }

    return 0;
}
