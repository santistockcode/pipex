#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <bsd/string.h>
#include "../include/libft.h"

// Use #defines for compile-time constants
#define SIZE_TEN 10
#define SRCSIZE_FIVE 5
#define SRC_FOR_DIFF_STRLCAT 10

// =======================
//  1) Testing ctype
// =======================
// void test_fts_ctype()
// {
//     int ch;

//     for (ch = 72; ch <= 75; ch++)
//     {
//         assert(isalpha(ch) == ft_isalpha(ch));
//         assert(isdigit(ch) == ft_isdigit(ch));
//         assert(isalnum(ch) == ft_isalnum(ch));
//         assert(isascii(ch) == ft_isascii(ch));
//         assert(isprint(ch) == ft_isprint(ch));
//         assert(toupper(ch) == ft_toupper(ch));
//         assert(tolower(ch) == ft_tolower(ch));
//     }
// }

// =======================
//  2) Example of testing strlcpy
// =======================
void test_apuntes(char *a, char *b, char *c, char *d)
{
    // We store original pointers for reference
    char *a_be = a;
    char *b_be = b;
    char *c_be = c;
    char *d_be = d;

    // same return value
    assert(ft_strlcpy(a, b, SIZE_TEN) == ft_strlcpy(c, d, SIZE_TEN));
    // arrays _before
    assert(strcmp(a_be, c_be) == 0);
    assert(strcmp(b_be, d_be) == 0);
    // arrays _after
    assert(strcmp(a, c) == 0);
    assert(strcmp(b, d) == 0);
    // size checks (note: sizeof(pointer) vs. arrays—be mindful)
}

// =======================
//  3) Test ft_bzero
// =======================
void test_ft_bzero()
{
    // Arrange
    char a[SIZE_TEN] = "AAAAAAAAA";
    char a_test[SIZE_TEN] = "AAAAAAAAA";
    size_t c;
    assert(strcmp(a, a_test) == 0);

    // Act
    bzero(a, SIZE_TEN);
    ft_bzero(a_test, SIZE_TEN);

    // Assert
    assert(sizeof(a) == sizeof(a_test));
    c = 0;
    while (c < sizeof(a))
    {
        assert(a[c] == a_test[c]);
        c++;
    }

    // Same test but using ints
    int b[SIZE_TEN] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int b_test[SIZE_TEN] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    bzero(b, 8);
    ft_bzero(b_test, 8);
    assert(sizeof(b) == sizeof(b_test));
    c = 0;
    while (c < SIZE_TEN)
    {
        assert(b[c] == b_test[c]);
        c++;
    }

    // Zero-size bzero
    char z[SIZE_TEN] = "AAAAAAAAA";
    char z_test[SIZE_TEN] = "AAAAAAAAA";
    assert(strcmp(z, z_test) == 0);

    bzero(z, 0);
    ft_bzero(z_test, 0);
    c = 0;
    while (c < sizeof(z))
    {
        assert(z[c] == z_test[c]);
        c++;
    }
}

// =======================
//  4) Test ft_strlcpy
// =======================
void test_ft_strlcpy()
{
    char a[SIZE_TEN] = "AAAAAAAAA";
    char a_test[SIZE_TEN] = "AAAAAAAAA";
    char b[5] = "bbbb";
    char b_test[5] = "bbbb";
    
    // Compare return values and resulting array
    assert(ft_strlcpy(a_test, b_test, 4) == ft_strlcpy(a, b, 4));
    assert(strcmp(a, a_test) == 0);

    // Another scenario
    char c[SIZE_TEN] = "aa";
    char c_test[SIZE_TEN] = "aa";
    char d[SIZE_TEN] = "ccccccccc";
    char d_test[SIZE_TEN] = "ccccccccc";
    assert(ft_strlcpy(c_test, d_test, 4) == ft_strlcpy(c, d, 4));
    assert(strcmp(c, c_test) == 0);

    // Copy empty string
    char e[SIZE_TEN] = "AAA";
    char e_test[SIZE_TEN] = "AAA";
    char f[5] = "";
    char f_test[5] = "";
    assert(ft_strlcpy(e_test, f_test, 10) == ft_strlcpy(e, f, 10));
    assert(strcmp(e, e_test) == 0);

    // dynamic allocation
    char *substr = malloc ((4 + 1) * sizeof(char)); 
	if (!substr) puts("error creating string");
    char *substr2 = malloc ((4 + 1) * sizeof(char)); 
	if (!substr2) puts("error creating string");

    assert(ft_strlcpy(substr, "hola", 4) == ft_strlcpy(substr2, "hola", 4));
    free(substr);
    free(substr2);
}

// =======================
//  5) Test ft_strlcat (abbreviated version shown)
// =======================
// void test_ft_strlcat()
// {
//     char a_dst[SRC_FOR_DIFF_STRLCAT];
//     char a_test_dst[SRC_FOR_DIFF_STRLCAT];

//     // ... Similar pattern of testing as strlcpy ...
//     // Make sure everything is properly sized and zero-terminated
//     // or at least safely used.
// }

// =======================
//  6) Test ft_strlen
// =======================
static void test_ft_strlen()
{
    char a_test[SIZE_TEN] = "1234567890";
    // Instead of zero-length array, use length 1 for an "empty" string
    char b_test[1] = ""; 

    assert(strlen(a_test) == ft_strlen(a_test));
    assert(strlen(b_test) == ft_strlen(b_test));

    // Another small array
    char c[2] = ""; // length 2, stores "" safely
    char c_test[2] = "";
    assert(strlen(c) == ft_strlen(c_test));
}

// =======================
//  7) Sample of other tests...
// =======================
void test_ft_strncmp()
{
    char a[SIZE_TEN] = "AAAAAAAAA";
    char a_test[SIZE_TEN] = "AAAAAAAAA";
    assert(strncmp(a, a_test, 0) == ft_strncmp(a, a_test, 0));
    // ...
}

// ... etc. for ft_strchr, ft_strrchr, ft_mem* tests, etc. ...

// =======================
//  Malloc / Free Tests
// =======================
void test_ft_calloc()
{
    int* arr1 = (int*)ft_calloc(10, sizeof(int));
    int* arr2 = (int*)calloc(10, sizeof(int));
    assert(memcmp(arr1, arr2, 10 * sizeof(int)) == 0);
    free(arr1);
    free(arr2);
}

// =======================
//  8) Finally, test_ft_substr
// =======================
void test_ft_substr()
{
    char *result = ft_substr("", 1, 1);
    assert(strcmp(result, "") == 0);
    free(result);
    // ...
    // (Remainder of your test code, no zero-length array usage)
}


// =======================
//  9) test_ft_split
// =======================
void test_ft_split()
{
    char buffer[] = "11111\n22222\n";
    char **result;

    result = ft_split(buffer, '\n');
    assert(ft_strlen(result[0]) == 5);
    for (int i = 0; i < 2; i++)
    {
        free(result[i]);
    }
    free(result);
}

// =======================
//  9) Main
// =======================
int main()
{
    // ctype.h
    // test_fts_ctype();

    // string.h
    test_ft_bzero();
    test_ft_strlen();
    test_ft_strncmp();
    test_ft_strlcpy();
    test_ft_split();

    test_ft_calloc();
    test_ft_substr();
    printf("\x1b[32mAll basic libft tests passed!\n\x1b[0m");

    return 0;
}
