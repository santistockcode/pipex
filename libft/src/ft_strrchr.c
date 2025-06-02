/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 15:27:46 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/14 15:27:49 by saalarco         ###   ########.fr       */
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
// #include <stdlib.h>

// size_t	ft_strlen(const char *s)
// {
// 	size_t	c;

// 	c = 0;
// 	while (*s++ != 0)
// 		c++;
// 	return (c);
// }

char	*ft_strrchr(const char *s, int c)
{
	char	ucc;
	int		cont;
	char	*rev;

	if (*s == 0 && c != '\0')
		return (NULL);
	ucc = (unsigned char) c;
	cont = (int) ft_strlen(s);
	rev = ((char *)(s + (cont)));
	while (*rev != ucc && cont > 0)
	{
		rev--;
		cont--;
	}
	if (*rev == ucc)
		return ((char *)rev);
	return (NULL);
}

// int main()
// {
// 	// Test case 1: Searching for 'a' in a string
//     const char *str1 = "Hello, world!";
//     int ch1 = 'a';
//     assert(ft_strrchr(str1, ch1) == strrchr(str1, ch1));
//     // Test case 2: Searching for 'o' in a string
//     const char *str2 = "Hello, world!";
//     int ch2 = 'o';
//     assert(ft_strrchr(str2, ch2) == strrchr(str2, ch2));
//     // Test case 3: Searching for 'z' in a string
//     const char *str3 = "Hello, world!";
//     int ch3 = 'z';
//     assert(ft_strrchr(str3, ch3) == strrchr(str3, ch3));
//     // Test case 4: Searching for '\0' in a string
// const char *str4 = "Hello, world!";
// int ch4 = '\0';
// assert(ft_strrchr(str4, ch4) == strrchr(str4, ch4));
// // Test case 5: Searching for 'H' in an empty string
// const char *str5 = "";
// int ch5 = 'H';
// assert(ft_strrchr(str5, ch5) == strrchr(str5, ch5));
// // Test case 6: Searching for 'o' in a null string
// // const char *str6 = NULL;
// // int ch6 = 'o';
// // assert(ft_strrchr(str6, ch6) == strrchr(str6, ch6));
// // Test case 7: Searching for '\0' in an empty string
// const char *str7 = "";
// int ch7 = '\0';
// assert(ft_strrchr(str7, ch7) == strrchr(str7, ch7));
// // puts(ft_strrchr(str7, ch7));
// // // Test case 8: Searching for 'z' in a null string
// // const char *str8 = "\0";
// // const char *str8_test = "\0";
// // int ch8 = 'z';
// // char *str8ans;
// // char *str8ans_test;
// // if (!(str8ans = strrchr(str8, ch8)))
// //     puts("NULL");
// // else
// //     puts(str8ans);
// // if (!(str8ans_test = ft_strrchr(str8_test, ch8)))
// //     puts("NULL");
// // else
// //     puts(str8ans_test);
// Test case 9: Searching for missing character
// const char *str9 = "bonjour";
// const char *str9_test = "bonjour";
// char *str9ans;
// char *str9ans_test;
// if (!(str9ans = strrchr(str9, 's')))
// 	puts("NULL");
// else
// 	puts(str9ans);
// if (!(str9ans_test = ft_strrchr(str9_test, 's')))
// 	puts("NULL");
// else
// 	puts(str9ans_test);
// printf("All tests passed successfully!\n");
// }
/// APUNTES PROPIOS
// en vez de : 	while (*rev != ucc && cont > 0) 
// posibilidad de hacerlo sin cambio de variable, 
// iterar cont-- atravesando *s + cont