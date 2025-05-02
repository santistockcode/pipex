/*
Exercise 5: Replacing Processes

    Goal: Learn execve and unlink.

    Write a C program that:

        Forks a child process.

        In the child process, use execve to replace it with the ls command.

        The parent process waits for the child to finish using wait.

    Add functionality to delete a temporary file using unlink.
    */


int main(void)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0)
    {
        // Error
        perror("fork failed in parent process");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        char *args[] = {"/bin/ls", "-l", NULL};
        execve("/bin/ls", args, NULL);
        perror("execve failed");
        return 1;
        // Delete temporary file
        unlink("temp_file.txt");
    }
    else
    {
        // Parent process
        if (wait(&status) < 0)
        {
            perror("wait failed in parent process");
            return 1;
        }
    }

    return 0;
}