#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <bsd/string.h>
#include "../include/libft.h"


const size_t SIZE_TEN = 10;
const size_t SRCSIZE_FIVE = 5;
const size_t SRC_FOR_DIFF_STRLCAT = 10;

//  Ctype
void test_fts_ctype()
{
    int ch;

    for (ch = -2; ch <= 200; ch++)
    {
        assert(isalpha(ch) == ft_isalpha(ch));
        assert(isdigit(ch) == ft_isdigit(ch));
        assert(isalnum(ch) == ft_isalnum(ch));
        assert(isascii(ch) == ft_isascii(ch));
        assert(isprint(ch) == ft_isprint(ch));
        assert(toupper(ch) == ft_toupper(ch));
        assert(tolower(ch) == ft_tolower(ch));
    }
}

// String manipulation
void test_apuntes(char *a, char *b, char *c, char *d)
{
    char *a_be = a;
    char *b_be = b;
    char *c_be = c;
    char *d_be = d;

    // same return value
    assert(strlcpy(a, b, SIZE_TEN) == strlcpy(c, d, SIZE_TEN));
    // arrays _before
    assert(strcmp(a_be, c_be) == 0);
    assert(strcmp(b_be, d_be) == 0);
    // arrays _after
   
    assert(strcmp(a, c) == 0);
    assert(strcmp(b, d) == 0);
    // sizeof
    assert(sizeof(a) == sizeof(b));
    assert(sizeof(c) == sizeof(d));
    // length
    assert(sizeof(a[0]) == sizeof(b[0]));
    assert(sizeof(c[0]) == sizeof(d[0]));
}

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

    // Assert and print
    assert(sizeof(a) == sizeof(a_test));
    //printf("%lu\n",sizeof(a_test));
    c = 0;
    while(c < sizeof(a)){
        assert(sizeof(a[c]) == sizeof(a_test[c]));
        assert(a[c] == a_test[c]);
        c++;
    }

    // Same test but using ints
    // Arrange
    int b[SIZE_TEN] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int b_test[SIZE_TEN] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Act
    bzero(b, 8);
    ft_bzero(b_test, 8);

    // Assert and print
    assert(sizeof(b) == sizeof(b_test));
    //printf("%lu\n",sizeof(a_test));
    c = 0;
    while(c < SIZE_TEN){
        assert(sizeof(b[c]) == sizeof(b_test[c]));
        assert(b[c] == b_test[c]);
        c++;
    }

    // Same test but with 0 on bzero
    // Arrange
    char z[SIZE_TEN] = "AAAAAAAAA";
    char z_test[SIZE_TEN] = "AAAAAAAAA";
    assert(strcmp(z, z_test) == 0);

    // Act
    bzero(z, (0));
    ft_bzero(z_test, 0);

    // Assert and print
    assert(sizeof(z) == sizeof(z_test));
    c = 0;
    while(c < sizeof(z)){
        assert(sizeof(z[c]) == sizeof(z_test[c]));
        assert(z[c] == z_test[c]);
        c++;
    }

    // Arrange
    char h[SIZE_TEN] = "";
    char h_test[SIZE_TEN] = "";
    assert(strcmp(h, h_test) == 0);

    // Act
    bzero(h, SIZE_TEN);
    ft_bzero(h_test, SIZE_TEN);
    assert(sizeof(h) == sizeof(h_test));
    assert(sizeof(h[0]) == sizeof(h_test[0]));

    // Arrange
    char i[SIZE_TEN] = "this";
    char i_test[SIZE_TEN] = "this";
    assert(strcmp(i, i_test) == 0);

    // Act
    bzero(i, 2);
    ft_bzero(i_test, 2);
    int count = 0;

    // Assert
    while(count < 2)
    {
        assert(i[count] == '\0');
        assert(i_test[count] == '\0');
        count++;
    }
    while(count < 4) /// solo hasta que terminan los char claro. 
    {
        assert(i[count] != '\0');
        assert(i_test[count] != '\0');
        count++;
    }
    while(count < 11)
    {
        assert(i[count] == '\0');
        assert(i_test[count] == '\0');
        count++;
    }
}

void test_ft_strlcpy()
{
    char a[SIZE_TEN] = "AAAAAAAAA";
    char a_test[SIZE_TEN] = "AAAAAAAAA";
    char b[5] = "bbbb";
    char b_test[5] = "bbbb";
    
    assert(ft_strlcpy(a_test, b_test, 4) == strlcpy(a, b, 4));
    assert(strcmp(a, a_test) == 0);

    char c[SIZE_TEN] = "aa";
    c[8] = '\0';
    c[8] = 'a';
    char c_test[SIZE_TEN] = "aa";
    c_test[8] = '\0';
    c_test[8] = 'a';
    char d[SIZE_TEN] = "ccccccccc";
    char d_test[SIZE_TEN] = "ccccccccc"; 
    assert(ft_strlcpy(c_test, d_test, 4) == strlcpy(c, d, 4));
    assert(strcmp(c, c_test) == 0);

    // string copy una cadena vacía
    char e[SIZE_TEN] = "AAA";
    char e_test[SIZE_TEN] = "AAA";
    char f[5] = "";
    char f_test[5] = "";
    
    assert(ft_strlcpy(e_test, f_test, 10) == strlcpy(e, f, 10));
    assert(strcmp(e, e_test) == 0);

    char *substr = malloc ((4 + 1) * sizeof(char)); 
	if (substr == NULL)
        puts("error creating string");
    
    char *substr2 = malloc ((4 + 1) * sizeof(char)); 
	if (substr == NULL)
        puts("error creating string");

    assert(ft_strlcpy(substr, "hola", 4) == strlcpy(substr2, "hola", 4));
    free(substr);
    free(substr2);
}

void test_ft_strlcat()
{
    char a_dst[SRC_FOR_DIFF_STRLCAT];
    char a_test_dst[SRC_FOR_DIFF_STRLCAT];
    char a1_dst[SRC_FOR_DIFF_STRLCAT];
    char a1_test_dst[SRC_FOR_DIFF_STRLCAT];
    char a2_dst[SRC_FOR_DIFF_STRLCAT];
    char a2_test_dst[SRC_FOR_DIFF_STRLCAT];
    char a3_dst[SRC_FOR_DIFF_STRLCAT];
    char a3_test_dst[SRC_FOR_DIFF_STRLCAT];
    char a4_dst[SRC_FOR_DIFF_STRLCAT];
    char a4_test_dst[SRC_FOR_DIFF_STRLCAT];

    // org1
    char b_org1[SIZE_TEN] = "AAAAAAAAA"; 
    char b_org2[SIZE_TEN] = ""; 
    char b_org3[SIZE_TEN] = "bbb"; 
    char b_org4[SIZE_TEN] = "AAAAAAAAAA"; 
    strlcpy(a_dst, b_org1, 0);
    strlcpy(a1_dst, b_org1, 1);
    strlcpy(a2_dst, b_org1, 8);
    strlcpy(a3_dst, b_org1, 9);
    strlcpy(a4_dst, b_org1, 10);
    
    ft_strlcpy(a_test_dst, b_org1, 0);
    ft_strlcpy(a1_test_dst, b_org1, 1);
    ft_strlcpy(a2_test_dst, b_org1, 8);
    ft_strlcpy(a3_test_dst, b_org1, 9);
    ft_strlcpy(a4_test_dst, b_org1, 10);

    assert(strcmp(a1_dst, a1_test_dst) == 0);
    assert(strcmp(a2_dst, a2_test_dst) == 0);
    assert(strcmp(a3_dst, a3_test_dst) == 0);
    assert(strcmp(a4_dst, a4_test_dst) == 0);

    char z_dst[SRC_FOR_DIFF_STRLCAT];
    char z_test_dst[SRC_FOR_DIFF_STRLCAT];
    char z1_dst[SRC_FOR_DIFF_STRLCAT];
    char z1_test_dst[SRC_FOR_DIFF_STRLCAT];
    char z2_dst[SRC_FOR_DIFF_STRLCAT];
    char z2_test_dst[SRC_FOR_DIFF_STRLCAT];
    char z3_dst[SRC_FOR_DIFF_STRLCAT];
    char z3_test_dst[SRC_FOR_DIFF_STRLCAT];
    char z4_dst[SRC_FOR_DIFF_STRLCAT];
    char z4_test_dst[SRC_FOR_DIFF_STRLCAT];

    assert(strlcpy(z_dst, b_org2, 0) == ft_strlcpy(z_test_dst, b_org2, 0));

    strlcpy(z1_dst, b_org2, 1);
    strlcpy(z2_dst, b_org2, 8);
    strlcpy(z3_dst, b_org2, 9);
    strlcpy(z4_dst, b_org2, 10);
    
    
    ft_strlcpy(z1_test_dst, b_org2, 1);
    ft_strlcpy(z2_test_dst, b_org2, 8);
    ft_strlcpy(z3_test_dst, b_org2, 9);
    ft_strlcpy(z4_test_dst, b_org2, 10);

    assert(strcmp(z1_dst, z1_test_dst) == 0);
    assert(strcmp(z2_dst, z2_test_dst) == 0);
    assert(strcmp(z3_dst, z3_test_dst) == 0);
    assert(strcmp(z4_dst, z4_test_dst) == 0);

    char y_dst[SRC_FOR_DIFF_STRLCAT];
    char y_test_dst[SRC_FOR_DIFF_STRLCAT];
    char y1_dst[SRC_FOR_DIFF_STRLCAT];
    char y1_test_dst[SRC_FOR_DIFF_STRLCAT];
    char y2_dst[SRC_FOR_DIFF_STRLCAT];
    char y2_test_dst[SRC_FOR_DIFF_STRLCAT];
    char y3_dst[SRC_FOR_DIFF_STRLCAT];
    char y3_test_dst[SRC_FOR_DIFF_STRLCAT];
    char y4_dst[SRC_FOR_DIFF_STRLCAT];
    char y4_test_dst[SRC_FOR_DIFF_STRLCAT];

    assert(strlcpy(y_dst, b_org3, 0) == ft_strlcpy(y_test_dst, b_org3, 0));

    strlcpy(y1_dst, b_org3, 1);
    strlcpy(y2_dst, b_org3, 8);
    strlcpy(y3_dst, b_org3, 9);
    strlcpy(y4_dst, b_org3, 10);
    
    
    ft_strlcpy(y1_test_dst, b_org3, 1);
    ft_strlcpy(y2_test_dst, b_org3, 8);
    ft_strlcpy(y3_test_dst, b_org3, 9);
    ft_strlcpy(y4_test_dst, b_org3, 10);

    assert(strcmp(y1_dst, y1_test_dst) == 0);
    assert(strcmp(y2_dst, y2_test_dst) == 0);
    assert(strcmp(y3_dst, y3_test_dst) == 0);
    assert(strcmp(y4_dst, y4_test_dst) == 0);

    char x_dst[SRC_FOR_DIFF_STRLCAT];
    char x_test_dst[SRC_FOR_DIFF_STRLCAT];
    char x1_dst[SRC_FOR_DIFF_STRLCAT];
    char x1_test_dst[SRC_FOR_DIFF_STRLCAT];
    char x2_dst[SRC_FOR_DIFF_STRLCAT];
    char x2_test_dst[SRC_FOR_DIFF_STRLCAT];
    char x3_dst[SRC_FOR_DIFF_STRLCAT];
    char x3_test_dst[SRC_FOR_DIFF_STRLCAT];
    char x4_dst[SRC_FOR_DIFF_STRLCAT];
    char x4_test_dst[SRC_FOR_DIFF_STRLCAT];

    assert(strlcpy(x_dst, b_org4, 0) == ft_strlcpy(x_test_dst, b_org4, 0));

    strlcpy(x1_dst, b_org4, 1);
    strlcpy(x2_dst, b_org4, 8);
    strlcpy(x3_dst, b_org4, 9);
    strlcpy(x4_dst, b_org4, 10);
    
    
    ft_strlcpy(x1_test_dst, b_org4, 1);
    ft_strlcpy(x2_test_dst, b_org4, 8);
    ft_strlcpy(x3_test_dst, b_org4, 9);
    ft_strlcpy(x4_test_dst, b_org4, 10);

    assert(strcmp(x1_dst, x1_test_dst) == 0);
    assert(strcmp(x2_dst, x2_test_dst) == 0);
    assert(strcmp(x3_dst, x3_test_dst) == 0);
    assert(strcmp(x4_dst, x4_test_dst) == 0);

    char w_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char w_test_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char w1_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char w1_test_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char w2_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char w2_test_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char w3_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char w3_test_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char w4_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char w4_test_dst[SRC_FOR_DIFF_STRLCAT] = "mark";

    char f_org1[SIZE_TEN] = "AAAAAAAA"; 

    assert(strlcpy(w_dst, f_org1, 0) == ft_strlcpy(w_test_dst, f_org1, 0));

    strlcpy(w1_dst, f_org1, 1);
    strlcpy(w2_dst, f_org1, 8);
    strlcpy(w3_dst, f_org1, 9);
    strlcpy(w4_dst, f_org1, 10);
    
    
    ft_strlcpy(w1_test_dst, f_org1, 1);
    ft_strlcpy(w2_test_dst, f_org1, 8);
    ft_strlcpy(w3_test_dst, f_org1, 9);
    ft_strlcpy(w4_test_dst, f_org1, 10);

    
    assert(strcmp(w1_dst, w1_test_dst) == 0);
    assert(strcmp(w2_dst, w2_test_dst) == 0);
    assert(strcmp(w3_dst, w3_test_dst) == 0);
    assert(strcmp(w4_dst, w4_test_dst) == 0);

    char v_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char v_test_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char v1_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char v1_test_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char v2_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char v2_test_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char v3_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char v3_test_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char v4_dst[SRC_FOR_DIFF_STRLCAT] = "mark";
    char v4_test_dst[SRC_FOR_DIFF_STRLCAT] = "mark";

    char g_org1[30] = "b";

    assert(strlcpy(v_dst, g_org1, 0) == ft_strlcpy(v_test_dst, g_org1, 0));

    strlcpy(v1_dst, g_org1, 1);
    strlcpy(v2_dst, g_org1, 8);
    strlcpy(v3_dst, g_org1, 9);
    strlcpy(v4_dst, g_org1, 10);
    
    
    ft_strlcpy(v1_test_dst, g_org1, 1);
    ft_strlcpy(v2_test_dst, g_org1, 8);
    ft_strlcpy(v3_test_dst, g_org1, 9);
    ft_strlcpy(v4_test_dst, g_org1, 10);

    assert(strcmp(v1_dst, v1_test_dst) == 0);
    assert(strcmp(v2_dst, v2_test_dst) == 0);
    assert(strcmp(v3_dst, v3_test_dst) == 0);
    assert(strcmp(v4_dst, v4_test_dst) == 0);

    char dest_extra2[30] = "rrrrrr";
    dest_extra2[14] = 'a';
    // printf("ORIGINAL:%zu\n",strlcat(dest_extra2, "lorem ipsum dolor sit amet", 15)); 
    char dest_extra3[30] = "rrrrrr";
    dest_extra3[14] = 'a';
    // printf("PROPIA:%zu\n",ft_strlcat(dest_extra3, "lorem ipsum dolor sit amet", 15)); 

    char dest_extra4[30] = "rrrrrr";
    dest_extra4[14] = 'a';
    // printf("ORIGINAL:%zu\n",strlcat(dest_extra4, "lorem ipsum dolor sit amet", 0)); 
    char dest_extra5[30] = "rrrrrr";
    dest_extra5[14] = 'a';
    // printf("PROPIA:%zu\n",ft_strlcat(dest_extra5, "lorem ipsum dolor sit amet", 0)); 
    
    char dest_extra6[30] = "rrrrrr";
    dest_extra6[14] = 'a';
    // printf("ORIGINAL:%zu\n",strlcat(dest_extra6, "lorem ipsum dolor sit amet", 1)); 
    char dest_extra7[30] = "rrrrrr";
    dest_extra7[14] = 'a';
    // printf("PROPIA:%zu\n",ft_strlcat(dest_extra7, "lorem ipsum dolor sit amet", 1)); 
    
}

/// String examination 
static void test_ft_strlen()
{
    char a_test[SIZE_TEN] = "1234567890";
    char b_test[0] = "";

    assert(strlen(a_test) == ft_strlen(a_test));
    assert(strlen(b_test) == ft_strlen(b_test));

    char c[0] = "";
    char c_test[0] = ""; 
    assert(strlen(c) == ft_strlen(c_test));
}

void test_ft_strncmp()
{
    // Arrange 
    char a[SIZE_TEN] = "AAAAAAAAAA";
    char a_test[SIZE_TEN] = "AAAAAAAAAA";

    // Act and assert and print
    //printf("PROPIA %i", ft_strncmp(a, a_test, SIZE));
    //printf("ORIGINAL %i", strncmp(a, a_test, SIZE));
    assert(strncmp(a, a_test, 0) == ft_strncmp(a, a_test, 0));
    assert(strncmp(a, a_test, 5) == ft_strncmp(a, a_test, 5));
    assert(strncmp(a, a_test, SIZE_TEN) == ft_strncmp(a, a_test, SIZE_TEN));
    
    // Act
        // diferente strings
    char b[SIZE_TEN] = "AAAAAAAAA";
    char b_test[SIZE_TEN] = "BBBBBBBBBB";

    assert(strncmp(b, b_test, 1) == ft_strncmp(b, b_test, 1));
    assert(strncmp(b, b_test, 5) == ft_strncmp(b, b_test, 5));
    assert(strncmp(b, b_test, SIZE_TEN) == ft_strncmp(b, b_test, SIZE_TEN));

    char c[SIZE_TEN] = "AAAAAAAAA";
    char c_test[SIZE_TEN] = "AAAAABAAAA";
    assert(strncmp(c, c_test, 1) == ft_strncmp(c, c_test, 1));
    assert(strncmp(c, c_test, 5) == ft_strncmp(c, c_test, 5));
    assert(strncmp(c, c_test, 4) == ft_strncmp(c, c_test, 4));
    assert(strncmp(c, c_test, 6) == ft_strncmp(c, c_test, 6));

    char d[SIZE_TEN] = "AAAAAAAAA";
    char d_test[SIZE_TEN] = "bAAAAAAAA";
    assert(strncmp(d, d_test, 0) == ft_strncmp(d, d_test, 0));
    assert(strncmp(d, d_test, 1) == ft_strncmp(d, d_test, 1));
    assert(strncmp(d, d_test, 5) == ft_strncmp(d, d_test, 5));
    assert(strncmp(d, d_test, 4) == ft_strncmp(d, d_test, 4));
    assert(strncmp(d, d_test, 6) == ft_strncmp(d, d_test, 6));

    char e[SIZE_TEN] = "AAAAAAAAA";
    char e_test[SIZE_TEN] = "AAAAAAAAB";
    assert(strncmp(e, e_test, 0) == ft_strncmp(e, e_test, 0));
    assert(strncmp(e, e_test, 1) == ft_strncmp(e, e_test, 1));
    assert(strncmp(e, e_test, 5) == ft_strncmp(e, e_test, 5));
    assert(strncmp(e, e_test, 10) == ft_strncmp(e, e_test, 10));
    assert(strncmp(e, e_test, 11) == ft_strncmp(e, e_test, 11));
        // diferent sizes
    char f[SIZE_TEN] = "AAAAAAAAB";
    char f_test[5] = "ABBA";
    assert(strncmp(f, f_test, 0) == ft_strncmp(f, f_test, 0));
    assert(strncmp(f, f_test, 1) == ft_strncmp(f, f_test, 1));
    assert(strncmp(f, f_test, 5) == ft_strncmp(f, f_test, 5));
    assert(strncmp(f, f_test, 10) == ft_strncmp(f, f_test, 10));
    assert(strncmp(f, f_test, 11) == ft_strncmp(f, f_test, 11));
    char g[5] = "ABBA";
    char g_test[SIZE_TEN] = "AAAAAAAAA";
    assert(strncmp(g, g_test, 0) == ft_strncmp(g, g_test, 0));
    assert(strncmp(g, g_test, 1) == ft_strncmp(g, g_test, 1));
    assert(strncmp(g, g_test, 5) == ft_strncmp(g, g_test, 5));
    assert(strncmp(g, g_test, 10) == ft_strncmp(g, g_test, 10));
    assert(strncmp(g, g_test, 11) == ft_strncmp(g, g_test, 11));

    // char h[4] = "AAA";
    // printf("ORIGIN: %i \n", strncmp(h, "", 0));
    // printf("ORIGIN: %i \n", strncmp(h, "", 5));
    // printf("PROPIA: %i \n", ft_strncmp(h, "", 0));
    // printf("PROPIA: %i \n", ft_strncmp(h, "", 5));
    // char i[4] = "AAA";
    // printf("ORIGIN: %i \n", strncmp("", i, 0));
    // printf("ORIGIN: %i \n", strncmp("", i, 5));
    // printf("PROPIA: %i \n", ft_strncmp("", i , 0));
    // printf("PROPIA: %i \n", ft_strncmp("", i, 5));

    // Test 11: si paso \200 y \0 debiere devolver 1 y no -1 
    //printf("RETURNS: %d", ft_strncmp("test\0", "test\200", 6));
}

void test_ft_strchr()
{
    // Arrange 
    char a[SIZE_TEN] = "aaaaaaaaa";
    int a_find = 97; // a
    char a_test[SIZE_TEN] = "aaaaaaaaa";
    int a_test_find = 97; // a
    //Act and assert and print
    // printf("ORIGIN: %s \n", strchr(a, a_find));
    // printf("PROPIA: %s \n", ft_strchr(a_test, a_test_find));
    assert(strcmp(strchr(a, a_find), ft_strchr(a_test, a_test_find)) == 0);

    // Buscando 0 char
    // Arrange 
    char c[SIZE_TEN] = "aaaaaaaaa";
    int c_find = 0;
    char c_test[SIZE_TEN] = "aaaaaaaaa";
    int c_test_find = 0;
    //Act and assert and print
    //printf("ORIGIN: %s \n", strchr(c, c_find));
    //printf("PROPIA: %s \n", ft_strchr(c_test, c_test_find));
    //assert(strcmp(strchr(a, a_find), ft_strchr(a_test, a_test_find)) == 0);
    assert(strcmp(strchr(c_test, c_test_find), "") == 0);
    assert(strcmp(ft_strchr(c, c_find), "") == 0);


    // Buscando algo que no está
    // Arrange 
    char b[SIZE_TEN] = "aaaaaaaaa";
    int b_find = 98;
    char b_test[SIZE_TEN] = "ccccccccc";
    int b_test_find = 98;
    // si añado una b al final de b, el strchr original concatena consigo mismo
    //b[9] = 98;
    //Act and assert and print (1)
    //printf("PROPIA: %s - ", ft_strchr(b, b_find));
    //printf("ORIGIN: %s \n", strchr(b_test, b_test_find));

    //Act and assert and print (1)
    //printf("ORIGIN: %s - ", strchr(b, b_find));
    //printf("PROPIA: %s \n", ft_strchr(b_test, b_test_find));

    assert(ft_strchr(b, b_find) == 0);
    assert(strchr(b_test, b_test_find) == 0);

    //assert(strcmp(ft_strchr(b, b_find), "") == 0);
    // char *str1;
    // char *str1_test;
    // char    str3[] = "";
    // if (!(str1 = strchr(str3, '\0')))
    //     // puts("ONULL");
    // else
    // {
    //     // puts(str1); // vaciísimo
    //     if (str1 != str3)
    //         // puts("\nOReturn value is false");
    // }
    // if (!(str1_test = ft_strchr(str3, '\0')))
    //     // puts("PNULL");
    // else
    // {
    //     // puts(str1_test);
    //     if (str1_test != str3)
    //         // puts("\nPReturn value is false");
    // }
    // // devuelvo null y debiere devolver 

    // test 2
    // char x[SIZE] = "aaaaaaaaa";
    // char x_test[SIZE] = "ccccccccc";
    // char *str_test;
    // char *str;

    // if (!(str = ft_strchr(x, '\0')))
    //     puts("ONULL");
    // else
    // {
    //     puts(str); 
    //     // if (str != x + 10)
    //     //     puts("\nOReturn value is false");
    // }

    // if (!(str_test = strchr(x_test, '\0')))
    //     puts("ONULL");
    // else
    // {
    //     puts(str_test); 
    //     // if(str_test != (x_test + 10))
    //     //     puts("\nOReturn value is false");
    // }

    // test 3
    char *y = "\0";
    char *d1 = strchr(y, 'a');
    char *d2 = ft_strchr(y, 'a');
    assert(d1 == d2);
    // char *z = "test";
    // char *d3 = strchr(z, '\0');
    // if (!(d3))
    //     puts("original NULL");
    // else
    // {
    //     puts("here it is d3");
    //     puts(d3); 
    //     if(d3 != (z + 4))
    //         puts("original Return value is false");
    //     else (puts("original Return value is true"));
    // }
    // char *d4 = ft_strchr(z, '\0');
    // if(!(d4))
    //     puts("propia NULL");
    // else
    // {
    //     puts("here it is d4");
    //     puts(d4); 
    //     if(d4 != (z + 4))
    //         puts("propia Return value is false");
    //     else (puts("propia Return value is true"));
    // }
    


}

void test_ft_strrchr()
{
    // Arrange 
    char a[SIZE_TEN] = "aaaaaaaaa";
    int a_find = 97; // a
    char a_test[SIZE_TEN] = "aaaaaaaaa";
    int a_test_find = 97; // a
    //Act and assert and print
    // printf("ORIGIN: %s \n", strchr(a, a_find));
    // printf("PROPIA: %s \n", ft_strchr(a_test, a_test_find));
    assert(strcmp(strrchr(a, a_find), ft_strrchr(a_test, a_test_find)) == 0);

    // Buscando 0 char
    // Arrange 
    char c[SIZE_TEN] = "aaaaaaaaa";
    int c_find = 0;
    char c_test[SIZE_TEN] = "aaaaaaaaa";
    int c_test_find = 0;
    //Act and assert and print
    //printf("ORIGIN: %s \n", strchr(c, c_find));
    //printf("PROPIA: %s \n", ft_strchr(c_test, c_test_find));
    //assert(strcmp(strchr(a, a_find), ft_strchr(a_test, a_test_find)) == 0);
    assert(strcmp(strrchr(c_test, c_test_find), "") == 0);
    assert(strcmp(ft_strchr(c, c_find), "") == 0);


    // Buscando algo que no está
    // Arrange 
    char b[SIZE_TEN] = "aaaaaaaaa";
    int b_find = 98;
    char b_test[SIZE_TEN] = "ccccccccc";
    int b_test_find = 98;
    // si añado una b al final de b, el strchr original concatena consigo mismo
    //b[9] = 98;
    //Act and assert and print (1)
    //printf("PROPIA: %s - ", ft_strchr(b, b_find));
    //printf("ORIGIN: %s \n", strchr(b_test, b_test_find));

    //Act and assert and print (1)
    //printf("ORIGIN: %s - ", strchr(b, b_find));
    //printf("PROPIA: %s \n", ft_strchr(b_test, b_test_find));

    assert(ft_strrchr(b, b_find) == 0);
    assert(strrchr(b_test, b_test_find) == 0);

    // Test 6: si se pasa un "\0" debe devolver el último y no uno random
    // char str_test6[8] = "rrrrrrr";
    // char *str_result_test6;
    // if (!(str_result_test6 = ft_strrchr(str_test6, '\0')))
    //     puts("NULL");
    // else
    // {
    //     puts(str_result_test6);
    //     if (str_result_test6 != str_test6 + 7)
    //         puts("\nReturn value is false");
    // }

}

void test_ft_strnstr()
{
    char a[SIZE_TEN] = "musica si";
    char a_test[SIZE_TEN] = "musica si";
    char b[3] = "si";
    char b_test[3] = "si";
    assert(strcmp(strnstr(a, b, 10),ft_strnstr(a_test, b_test, 10)) == 0);

    char c[SIZE_TEN] = "si musica";
    char c_test[SIZE_TEN] = "si musica";
    char d[3] = "si";
    char d_test[3] = "si";
    assert(strcmp(strnstr(c, d, 10),ft_strnstr(c_test, d_test, 10)) == 0);

    char e[SIZE_TEN] = "si mus si";
    char e_test[SIZE_TEN] = "si mus si";
    char f[3] = "si";
    char f_test[3] = "si";
    assert(strcmp(strnstr(e, f, 10),ft_strnstr(e_test, f_test, 10)) == 0);

    char g[SIZE_TEN] = "ss mus si";
    char g_test[SIZE_TEN] = "ss mus si";
    char h[3] = "si";
    char h_test[3] = "si";
    assert(strcmp(strnstr(g, h, 10),ft_strnstr(g_test, h_test, 10)) == 0);

    char i[SIZE_TEN] = "ss mus si";
    char i_test[SIZE_TEN] = "ss mus si";
    char j[3] = "s";
    char j_test[3] = "s";
    assert(strcmp(strnstr(i, j, 10),ft_strnstr(i_test, j_test, 10)) == 0);

    char k[SIZE_TEN] = "si mus si";
    char k_test[SIZE_TEN] = "si mus si";
    char l[3] = "i";
    char l_test[3] = "i";
    assert(strcmp(strnstr(k, l, 10),ft_strnstr(k_test, l_test, 10)) == 0);

    char m[SIZE_TEN] = "musica si";
    char m_test[SIZE_TEN] = "musica si";
    char n[3] = "i";
    char n_test[3] = "i";
    assert(strcmp(strnstr(m, n, 10),ft_strnstr(m_test, n_test, 10)) == 0);

    char o[SIZE_TEN] = "casi";
    char o_test[SIZE_TEN] = "casi";
    char p[2] = "c";
    char p_test[2] = "c";
    assert(strcmp(strnstr(o, p, 10),ft_strnstr(o_test, p_test, 10)) == 0);

    // cases from francinette
	char haystack[30] = "aaabcabcd";
	char needle[10] = "aabc";
	char * empty = (char*)"";
	assert(ft_strnstr(haystack, needle, 0) == 0);
	assert(ft_strnstr(haystack, needle, -1) == haystack + 1); 
	assert(ft_strnstr(haystack, "a", -1) == haystack);
	assert(ft_strnstr(haystack, "c", -1) == haystack + 4);
	assert(ft_strnstr(empty, "", -1) == empty);
	assert(ft_strnstr(empty, "", 0) == empty);
	assert(ft_strnstr(empty, "coucou", -1) == 0);
	assert(ft_strnstr(haystack, "aaabc", 5) == haystack);
	assert(ft_strnstr(empty, "12345", 5) == 0);
	assert(ft_strnstr(haystack, "abcd", 9) == haystack + 5);
	assert(ft_strnstr(haystack, "cd", 8) == NULL);
	assert(ft_strnstr(haystack, "a", 1) == haystack);
	assert(ft_strnstr("1", "a", 1) == NULL);
	assert(ft_strnstr("22", "b", 2) == NULL);
}

// Character array manipulation 
void test_ft_memchr()
{
    // // MISMA PRUEBA PERO CON INTS
    // // Arrange
    // // int b[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // // int b_test[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // // //assert(strcmp(a, a_test) == 0);

    // // // Act
    // // printf("%p \n", memchr(b, 97, 8));
    // // printf("%p \n", ft_memchr(b_test, 97, 8));

    // // Arrange
    // char a[SIZE] = "AAAAAAAAA";
    // char a_test[SIZE] = "AAAAAAAAA";

    // //printf("%s \n", memchr(a, 65, 8));
    // //printf("%s \n", ft_memchr(a_test, 65, 8));
    // assert(strcmp(memchr(a, 65, 10), ft_memchr(a_test, 65, 10)) == 0);

    // // Arrange
    // //char c[SIZE] = "AACCAABB";
    // //char c_test[SIZE] = "AACCAABB";

    // //printf("%s \n", memchr(c, 66, 5));
    // //printf("%s \n", ft_memchr(c_test, 66, 5));
    // // esto debiere dar segmentation fault (por devolver NULL)
    // //assert(strcmp(memchr(c, 66, 1), ft_memchr(c_test, 66, 1)) == 0);

    // // memchr chrash because it reads too many bytes from s or attempt to write on s! 
    // int size = 15;
    // char *src = malloc(2 * sizeof(char));
    // //memchr(src, 200, size);
    // ft_memchr(src, 200, size);

    char s[] = {0, 1, 2 ,3 ,4 ,5};
    assert(ft_memchr(s, 0, 0) == NULL);
	assert(ft_memchr(s, 0, 1) == s);
    //char *res = ft_memchr(s, 2, 3);
	assert(ft_memchr(s, 2, 3) == s + 2);
	assert(ft_memchr(s, 6, 6) == NULL);
	assert(ft_memchr(s, 2 + 256, 3) == s + 2); 
    // ft_memchr(((void *)0), '\0', 0x20);
}

void test_ft_memcpy()
{
    char origen[10] = "AAAAAAAAA";
    char destino[10];
    char origen_test[10] = "AAAAAAAAA";
    char destino_test[10];
    int tamanio; 

    tamanio = 1;
    // printf("%s \n", memcpy(destino, origen, tamanio));
    // printf("%s \n", ft_memcpy(destino_test, origen_test, tamanio));
    memcpy(destino, origen, tamanio);
    ft_memcpy(destino_test, origen_test, tamanio);
    int c = 0;

    while(c < tamanio)
    {
        assert(destino[c] == destino_test[c]);
        c++;
    }

    //char o1[10] = "AAAAAAAAA";
    char d1[10] = "BBBBBBBBB";
    // char o1_test[10] = "AAAAAAAAA";
    char d1_test[10] = "BBBBBBBBB";
    int t; 

    t = 4;
    // printf("%s \n", memcpy(destino, origen, tamanio));
    // printf("%s \n", ft_memcpy(destino_test, origen_test, tamanio));
    memcpy(d1, d1, t);
    ft_memcpy(d1_test, d1_test, t);
    int count = 0;

    while(count < t)
    {
        assert(d1[count] == d1_test[count]);
        count++;
    }

    // Test case 1: Copying a string
    char src1[] = "Hello, world!";
    char dest1[20];
    memcpy(dest1, src1, sizeof(src1));
    ft_memcpy(dest1, src1, sizeof(src1));
    assert(memcmp(dest1, src1, sizeof(src1)) == 0);

    // Test case 2: Copying an array of integers
    int src2[] = {1, 2, 3, 4, 5};
    int dest2[5];
    memcpy(dest2, src2, sizeof(src2));
    ft_memcpy(dest2, src2, sizeof(src2));
    assert(memcmp(dest2, src2, sizeof(src2)) == 0);

    // Test case 3: Copying a struct
    struct {
        int x;
        int y;
    } src3 = {10, 20};
    struct {
        int x;
        int y;
    } dest3;
    memcpy(&dest3, &src3, sizeof(src3));
    ft_memcpy(&dest3, &src3, sizeof(src3));
    assert(memcmp(&dest3, &src3, sizeof(src3)) == 0);

    // Test case 4: NULL as both params with size
    void *src4 = (void *)0;
    void *dest4 = (void *)0;
    memcpy(dest4, src4, 3);
    ft_memcpy(dest4, src4, 3);
    //assert(memcmp(dest4, src4, sizeof(src4)) == 0);

    // Test case 5: null as firstt parameter
    // void *src5 = (void *)0;
    // char *dest5 = "                 ";
    //memcpy(dest5, src5, 3);
    //ft_memcpy(dest5, src5, 3);
    //assert(memcmp(dest4, src4, sizeof(src4)) == 0);

}

void test_ft_memcmp()
{
    char a[10] = "HCTIBBACA";
    char b[10] = "AAAABAAAA";
    assert(memcmp(a, b, 1) == ft_memcmp(a, b, 1));
    assert(memcmp(a, b, 5) == ft_memcmp(a, b, 5));
    assert(memcmp(a, b, 10) == ft_memcmp(a, b, 10));

    int e[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int f[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(memcmp(e, f, 1) == ft_memcmp(e, f, 1));
    assert(memcmp(e, f, 5) == ft_memcmp(e, f, 5));
    assert(memcmp(e, f, 10) == ft_memcmp(e, f, 10));

    char c[10] = "AAABAAAAA";
    char d[10] = "AAAAAAAAA";
    assert(memcmp(c, d, 0) == ft_memcmp(c, d, 0));
    assert(memcmp(c, d, 5) == ft_memcmp(c, d, 5));
    assert(memcmp(c, d, 10) == ft_memcmp(c, d, 10));
    // Test case 1: Comparing equal strings
    char str1[] = "Hello";
    char str2[] = "Hello";
    assert(memcmp(str1, str2, sizeof(str1)) == ft_memcmp(str1, str2, sizeof(str1)));

    // Test case 2: Comparing different strings
    char str3[] = "Hello";
    char str4[] = "World";
    assert(memcmp(str3, str4, sizeof(str3)) == ft_memcmp(str3, str4, sizeof(str3)));

    // Test case 3: Comparing equal arrays of integers
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};
    assert(memcmp(arr1, arr2, sizeof(arr1)) == ft_memcmp(arr1, arr2, sizeof(arr1)));

    // Test case 4: Comparing different arrays of integers
    int arr3[] = {1, 2, 3, 4, 5};
    int arr4[] = {5, 4, 3, 2, 1};
    assert(memcmp(arr3, arr4, sizeof(arr3)) == ft_memcmp(arr3, arr4, sizeof(arr3)));
}

void test_ft_memset()
{
    char b[SIZE_TEN];
    char b_test[SIZE_TEN];
    //assert(strcmp(memset(b, 97, 5), ft_memset(b_test, 97, 5)) == 0);
    //assert(strcmp(memset(b, 0, 5), ft_memset(b_test, 0, 5)) == 0);

    assert(strcmp(memset(b, 97, 9), ft_memset(b_test, 97, 9)) == 0);
    // si es el SIZE completo, entonces falla
    //assert(strcmp(memset(b, 97, SIZE), ft_memset(b_test, 97, SIZE)) == 0);

}

void test_ft_memmove()
{
    // Test case 1: Moving data within the same buffer
    char buffer1[] = "Hello, World!";
    char buffer2[] = "Hello, World!";
    size_t n = 5;
    ft_memmove(buffer1 + 3, buffer1, n);
    memmove(buffer2 + 3, buffer2, n);

    // puts(buffer1);
    // puts(buffer2);
    
    // assert(strcmp(buffer1, buffer2) == 0);

    // Test case 2: Moving data from one buffer to another
    char src[] = "Hello";
    char dest1[10];
    char dest2[10];
    ft_memmove(dest1, src, strlen(src) + 1);
    memmove(dest2, src, strlen(src) + 1);
    assert(strcmp(dest1, dest2) == 0);

    // Test case 3: Moving overlapping data
    char buffer3[] = "Hello, World!";
    char buffer4[] = "Hello, World!";
    size_t n2 = 7;
    ft_memmove(buffer3 + 3, buffer3 + 6, n2);
    memmove(buffer4 + 3, buffer4 + 6, n2);
    assert(strcmp(buffer3, buffer4) == 0);
}

// Conversions to and from numeric formats 
void test_ft_atoi(const char *str)
{   int expected = atoi(str);
    int result = ft_atoi(str);
    assert(result == expected);
    // printf("Test passed: ft_atoi(\"%s\") = %d\n", str, result);
}

void test_ft_calloc()
{
    // Test case 1: Allocating and initializing memory blocks of size 10
    int* arr1 = (int*)ft_calloc(10, sizeof(int));
    int* arr2 = (int*)calloc(10, sizeof(int));
    assert(memcmp(arr1, arr2, 10 * sizeof(int)) == 0);
    free(arr1);
    free(arr2);

    // Test case 2: Allocating and initializing memory blocks of size 100
    char* str1 = (char*)ft_calloc(100, sizeof(char));
    char* str2 = (char*)calloc(100, sizeof(char));
    assert(memcmp(str1, str2, 100 * sizeof(char)) == 0);
    free(str1);
    free(str2);

    // // // Test case 3: Allocating and initializing memory blocks of size 5
    // float* floatArr1 = (float*)ft_calloc(5, sizeof(float));
    // float* floatArr2 = (float*)calloc(5, sizeof(float));
    // printf("%fu", floatArr1[0]);
    // printf("%fu", floatArr2[0]);
    // assert(memcmp(floatArr1, floatArr2, 5 * sizeof(float)) == 0);
    // free(floatArr1);
    // free(floatArr2);

    // Test case 4: Allocating and initializing memory blocks of size 0
    // int* arr3 = (int*)ft_calloc(0, sizeof(int));
    // int* arr4 = (int*)calloc(0, sizeof(int));
    // printf("%lu",sizeof(arr3));
    // printf("%lu",sizeof(arr4));
    // printf("%d", *arr3);
    // printf("%d", *arr4);

    // Test case 5: Allocating and initializing 0 memory blocks
    char* str3 = (char*)ft_calloc(10, 0);
    char* str4 = (char*)calloc(10, 0);
    assert(str3 != NULL && str4 != NULL);   
}

void test_ft_strdup()
{
    // Test case 1: Empty string
    const char *str1 = "";
    char *result1 = ft_strdup(str1);
    assert(strcmp(result1, str1) == 0);
    free(result1);

    // Test case 2: Non-empty string
    const char *str2 = "Hello, world!";
    char *result2 = ft_strdup(str2);
    assert(strcmp(result2, str2) == 0);
    free(result2);

    // Test case 3: String with special characters
    const char *str3 = "Testing\tnewline\nand\rreturn\r\ncharacters";
    char *result3 = ft_strdup(str3);
    assert(strcmp(result3, str3) == 0);
    free(result3);

    // Test case 4: String with null character
    const char *str4 = "This\0is\0a\0test";
    char *result4 = ft_strdup(str4);
    assert(strcmp(result4, str4) == 0);
    free(result4);

    // Test case 5: String with only null characters
    const char *str5 = "\0\0\0\0\0\0\0\0\0\0";
    char *result5 = ft_strdup(str5);
    char *result5_test = strdup(str5);
    assert(strcmp(result5, result5_test) == 0);
    free(result5);

    // Test case 6: string empty ""
    const char *str6 = "";
    char *result6 = ft_strdup(str6);
    char *restult6_test = strdup(str6);
    assert(strcmp(result6, restult6_test) == 0);
    free(result6);

    // Test case 7: string with fdfjkdf\n35346
    const char *str7 = "fdfjkdf\n35346";
    char *result7 = ft_strdup(str7);
    char *restult7_test = strdup(str7);
    assert(strcmp(result7, restult7_test) == 0);
    free(result7);
}

void test_ft_substr()
{
// cases from francinette
assert(strcmp(ft_substr("", 1, 1), "") == 0);
assert(strcmp(ft_substr("", 0, 0), "") == 0);
assert(strcmp(ft_substr("", 0, 1), "") == 0);
assert(strcmp(ft_substr("", 1, 1), "") == 0); 
assert(strcmp(ft_substr("hola", -1, 0), "") == 0);
assert(strcmp(ft_substr("hola", 0, -1), "hola") == 0);
assert(strcmp(ft_substr("hola", -1, -1), "") == 0);
assert(strcmp(ft_substr("hola", 0, 0), "") == 0);
assert(strcmp(ft_substr("hola", 0, 1), "h") == 0);
assert(strcmp(ft_substr("hola", 0, 3), "hol") == 0);
assert(strcmp(ft_substr("hola", 0, 4), "hola") == 0);
assert(strcmp(ft_substr("hola", 0, 5), "hola") == 0);
assert(strcmp(ft_substr("hola", 2, 0), "") == 0);
assert(strcmp(ft_substr("hola", 2, 1), "l") == 0);
assert(strcmp(ft_substr("hola", 2, 2), "la") == 0);
assert(strcmp(ft_substr("hola", 2, 3), "la") == 0);
assert(strcmp(ft_substr("hola", 2, 30), "la") == 0);
assert(strcmp(ft_substr("hola", 3, 0), "") == 0);
assert(strcmp(ft_substr("hola", 3, 1), "a") == 0);
assert(strcmp(ft_substr("hola", 3, 2), "a") == 0);
assert(strcmp(ft_substr("hola", 4, 0), "") == 0);
assert(strcmp(ft_substr("hola", 4, 1), "") == 0);
assert(strcmp(ft_substr("hola", 4, 20), "") == 0);
assert(strcmp(ft_substr("hola", 5, 2), "") == 0);

// TEST 1 (test 2 es lo mismo pero el argumentos son 7 y 10 en vez de 0 y 10)
char	str[] = "lorem ipsum dolor sit amet";
char	*strsub;
if (!(strsub = ft_substr(str, 7, 10)))
		printf("%s", "null");
	else
		//printf("%s",strsub);
	if (str == strsub)
		printf("%s","\nA new string was not returned");



char	str_test[] = "lorem ipsum dolor sit amet";
char	*strsub_test;
if (!(strsub_test = ft_substr(str_test, 7, 10)))
		printf("%s", "null");
	else
		//printf("%s",strsub_test);
	if (str_test == strsub_test)
		printf("%s","\nA new string was not returned");
// expected: lorem ipsu (for 0 and 10)
// expected: psum dolor (for 7 and 10)
/*
Lo devuelve correctamente osea que puede ser el \0 que no haya que ponerlo
*/
}

void test_ft_itoa()
{
    // -2147483647
    assert(strcmp(ft_itoa(-2147483647), "-2147483647") == 0);
    // 2147483647
    assert(strcmp(ft_itoa(2147483647), "2147483647") == 0);
    // -2147483648 fuera del range
    // assert(strcmp(ft_itoa(-2147483648), "-2147483648") == 0);
    // 2147483648 fuera del range
    // assert(strcmp(ft_itoa(2147483648), "2147483648") == 0);
    // 0
    assert(strcmp(ft_itoa(0), "0") == 0);
    // -1
    assert(strcmp(ft_itoa(-1), "-1") == 0);
    // 1
    assert(strcmp(ft_itoa(1), "1") == 0);
}

// main
int main()
{
    // ctype.h
    test_fts_ctype();

    // string.h
    test_ft_bzero();
    test_ft_strlen();
    test_ft_strncmp();
    test_ft_strchr();
    test_ft_strrchr();
    test_ft_memset();
    test_ft_memchr();
    test_ft_memcpy();
    test_ft_memcmp();
    test_ft_memmove();
    test_ft_strlcpy();
    test_ft_strlcat();
    test_ft_strnstr();

    //atoi
    test_ft_atoi("-456");
    test_ft_atoi("0");
    test_ft_atoi("42");
    test_ft_atoi("+987");
    test_ft_atoi(" +-123junk");
    test_ft_atoi("+0");
    test_ft_atoi("0042");
    test_ft_atoi("0x2A");
    test_ft_atoi("junk");
    test_ft_atoi("2147483647");
    test_ft_atoi("2147483649");
    test_ft_atoi("-2147483648");
    test_ft_atoi("\t\n\r\v\f 1586 \n");

    // Malloc using functions
    test_ft_calloc();
    test_ft_strdup();

    // Second part
    test_ft_substr();
    test_ft_itoa();
    printf("\x1b[32mTests propios básicos para la libft OK\n");

    return (0);
}
