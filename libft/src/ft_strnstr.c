/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 15:36:55 by saalarco          #+#    #+#             */
/*   Updated: 2024/02/14 10:14:36 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// #include <assert.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <ctype.h>
// #include <limits.h>
// #include <string.h>
// #include <unistd.h>
// #include <stddef.h>
// #include <stdlib.h>
// const size_t SIZE_TEN = 10;
// const size_t SRCSIZE_FIVE = 5;
// const size_t SRC_FOR_DIFF_STRLCAT = 10;

// size_t	ft_strlen(const char *s)
// {
// 	size_t	c;

// 	c = 0;
// 	while (*s++ != '\0')
// 		c++;
// 	return (c);
// }

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	max;
	size_t	min_needle;
	size_t	paso_len;

	if (*needle == '\0' || len == 0)
		return ((char *) haystack);
	if (len > ft_strlen(haystack))
		len = ft_strlen(haystack);
	max = len;
	min_needle = 0;
	while (len != 0 && haystack[max - len] != '\0')
	{
		paso_len = len;
		while (len != 0 && haystack[max - len] == needle[min_needle])
		{
			len--;
			min_needle++;
		}
		if (needle[min_needle] == '\0' && len == (paso_len - min_needle))
			return ((char *) haystack + (max - paso_len));
		len = paso_len;
		min_needle = 0;
		len--;
	}
	return (0);
}
/*
Characters that appear after a `\0' character are not searched
Si la longitud a buscar ya es menor que needle poco vamos a encontrar.
estaría mas optimizada teniendo encuenta si len <= hlen (no lo va a encontrar)
needle = n; es para rebobinar needle
not used:
size_t	nlen;
nlen = aux_strnstr_ft_strlen(needle);
*/

// int main()
// {
// 	char a[SIZE_TEN] = "musica si";
//     char a_test[SIZE_TEN] = "musica si";
//     char b[3] = "si";
//     char b_test[3] = "si";
//     assert(strcmp(strnstr(a, b, 10),ft_strnstr(a_test, b_test, 10)) == 0);

//     char c[SIZE_TEN] = "si musica";
//     char c_test[SIZE_TEN] = "si musica";
//     char d[3] = "si";
//     char d_test[3] = "si";
//     assert(strcmp(strnstr(c, d, 10),ft_strnstr(c_test, d_test, 10)) == 0);

//     char e[SIZE_TEN] = "si mus si";
//     char e_test[SIZE_TEN] = "si mus si";
//     char f[3] = "si";
//     char f_test[3] = "si";
//     assert(strcmp(strnstr(e, f, 10),ft_strnstr(e_test, f_test, 10)) == 0);

//     char g[SIZE_TEN] = "ss mus si";
//     char g_test[SIZE_TEN] = "ss mus si";
//     char h[3] = "si";
//     char h_test[3] = "si";
//     assert(strcmp(strnstr(g, h, 10),ft_strnstr(g_test, h_test, 10)) == 0);

//     char i[SIZE_TEN] = "ss mus si";
//     char i_test[SIZE_TEN] = "ss mus si";
//     char j[3] = "s";
//     char j_test[3] = "s";
//     assert(strcmp(strnstr(i, j, 10),ft_strnstr(i_test, j_test, 10)) == 0);

//     char k[SIZE_TEN] = "si mus si";
//     char k_test[SIZE_TEN] = "si mus si";
//     char l[3] = "i";
//     char l_test[3] = "i";
//     assert(strcmp(strnstr(k, l, 10),ft_strnstr(k_test, l_test, 10)) == 0);

//     char m[SIZE_TEN] = "musica si";
//     char m_test[SIZE_TEN] = "musica si";
//     char n[3] = "i";
//     char n_test[3] = "i";
//     assert(strcmp(strnstr(m, n, 10),ft_strnstr(m_test, n_test, 10)) == 0);

//     char o[SIZE_TEN] = "casi";
//     char o_test[SIZE_TEN] = "casi";
//     char p[2] = "c";
//     char p_test[2] = "c";
//     assert(strcmp(strnstr(o, p, 10),ft_strnstr(o_test, p_test, 10)) == 0);

// 	char haystack[30] = "aaabcabcd";
// 	char needle[10] = "aabc";
// 	char * empty = (char*)"";
// 	assert(ft_strnstr(haystack, needle, 0) == 0);
// 	assert(ft_strnstr(haystack, needle, -1) == haystack + 1); 
// 	assert(ft_strnstr(haystack, "a", -1) == haystack);
// 	assert(ft_strnstr(haystack, "c", -1) == haystack + 4);
// 	assert(ft_strnstr(empty, "", -1) == empty);
// 	assert(ft_strnstr(empty, "", 0) == empty);
// 	assert(ft_strnstr(empty, "coucou", -1) == 0);
// 	assert(ft_strnstr(haystack, "aaabc", 5) == haystack);
// 	assert(ft_strnstr(empty, "12345", 5) == 0);
// 	assert(ft_strnstr(haystack, "abcd", 9) == haystack + 5);
// 	assert(ft_strnstr(haystack, "cd", 8) == NULL);
// 	assert(ft_strnstr(haystack, "a", 1) == haystack);
// 	assert(ft_strnstr("1", "a", 1) == NULL);
// 	assert(ft_strnstr("22", "b", 2) == NULL);

// 	printf("All tests passed succesfully");
// }
