#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
/*
Exercise 2: Duplicating File Descriptors

    Goal: Learn dup and dup2.

    Write a C program that:

        Opens a file for writing.

        Duplicates stdout using dup or dup2 so that anything written to stdout goes 
        into the file instead of the terminal.

        Print some text to stdout and confirm it appears in the file.
        */


int main(void)
{
    int fd;

    fd = open("fileout", O_WRONLY);
    dup2(fd, STDOUT_FILENO);
    /*
    Esto también se puede hacer con dup en vez de dup2, si cierro primero STDOUT_FILENO
    , luego haces dup y como toma el fd más bajito toma 1 
    dup(fd) y ya todo lo que se escriba a STDOUT (1) pues irá a 1 que ahora es mi fd*/

    close(fd);

    printf("Hello, world!\n");

    return(0);
}