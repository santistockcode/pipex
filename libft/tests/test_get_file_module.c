#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../include/libft.h"
#include "../include/get_file.h"
#include "../include/ft_printf.h"

static void test_ft_file_read_all(void)
{
    int fd = open("./tests/example_get_file.txt", O_RDONLY);
    assert(fd != -1 && "Could not open example.txt for testing");

    t_file *file = ft_file_read_all(fd);
    close(fd);

    // Minimal checks
    assert(file != NULL && "ft_file_read_all returned NULL");
    assert(file->line_count > 0 && "Expected some lines in example.txt");
    // Possibly more checks on file->buffer, etc.

    ft_file_free(&file);
    assert(file == NULL && "ft_file_free did not set file pointer to NULL");
}

static void test_ft_file_print_buffer(void)
{
    int fd = open("./tests/example_get_file.txt", O_RDONLY);
    assert(fd != -1);

    t_file *file = ft_file_read_all(fd);
    close(fd);

    // Just call the function. We can't "assert" printing, but we can see if it crashes.
    ft_file_print_buffer(file);

    ft_file_free(&file);
}

int main(void)
{
    test_ft_file_read_all();
    test_ft_file_print_buffer();

    printf("\x1b[32mAll get file module tests passed!\n\x1b[0m");
    return 0;
}
