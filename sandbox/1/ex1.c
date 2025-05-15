#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/*
Exercise 1: Working with Files

    Goal: Understand open, close, read, write, and strerror.

    Create a file called example.txt with some text in it.

    Write a C program to:

        Open the file with open.

        Read its contents using read.

        Write the contents to stdout using write.

        Handle errors using strerror if the file cannot be opened.

    Close the file with close.*/



int main(int argc, char **argv)
{
    int fd;
    char buffer[200];
    int nread;
    int i;

    if (argc != 2)
        return (0);
    fd = open (argv[1], O_RDONLY);
    if (fd == -1)
        return (write(1, "error", 6), (0));
    i = 0;
    nread = read(fd, buffer, 200);
    while(i < nread)
    {
        write(1, &buffer[i], 1);
        i++;
    }



}

