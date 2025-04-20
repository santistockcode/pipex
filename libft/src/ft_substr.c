/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 16:49:33 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/06 11:22:43 by saalarco         ###   ########.fr       */
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

static size_t	calc_len(unsigned int start, size_t slen, size_t len)
{
	if (start >= slen)
		len = 0;
	if (len > slen - start)
		len = slen - start;
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	slen;
	size_t	count;

	if (s == NULL)
		return (NULL);
	slen = ft_strlen(s);
	len = calc_len(start, slen, len);
	substr = malloc ((len + 1) * sizeof(char));
	if (substr == NULL)
		return (NULL);
	if (start >= slen)
	{
		*substr = '\0';
		return (substr);
	}
	count = 0;
	while (count < len)
	{
		substr[count] = s[count + start];
		count++;
	}
	substr[count] = '\0';
	return (substr);
}

/*
s: La string desde la que crear la substring.
start: El índice del caracter en ’s’ desde el que
empezar la substring.
len: La longitud máxima de la substring.
si el índice no está en la cadena entonces la devuelve vacía
*/

// int main()
// {
// // TESTS cortos
// assert(strcmp(ft_substr("", 1, 1), "") == 0);
// assert(strcmp(ft_substr("", 0, 0), "") == 0);
// assert(strcmp(ft_substr("", 0, 1), "") == 0);
// assert(strcmp(ft_substr("", 1, 1), "") == 0); 
// assert(strcmp(ft_substr("hola", -1, 0), "") == 0);
// assert(strcmp(ft_substr("hola", 0, -1), "hola") == 0);
// assert(strcmp(ft_substr("hola", -1, -1), "") == 0);
// assert(strcmp(ft_substr("hola", 0, 0), "") == 0);
// assert(strcmp(ft_substr("hola", 0, 1), "h") == 0);
// assert(strcmp(ft_substr("hola", 0, 3), "hol") == 0);
// assert(strcmp(ft_substr("hola", 0, 4), "hola") == 0);
// assert(strcmp(ft_substr("hola", 0, 5), "hola") == 0);
// assert(strcmp(ft_substr("hola", 2, 0), "") == 0);
// assert(strcmp(ft_substr("hola", 2, 1), "l") == 0);
// assert(strcmp(ft_substr("hola", 2, 2), "la") == 0);
// assert(strcmp(ft_substr("hola", 2, 3), "la") == 0);
// assert(strcmp(ft_substr("hola", 2, 30), "la") == 0);
// assert(strcmp(ft_substr("hola", 3, 0), "") == 0);
// assert(strcmp(ft_substr("hola", 3, 1), "a") == 0);
// assert(strcmp(ft_substr("hola", 3, 2), "a") == 0);
// assert(strcmp(ft_substr("hola", 4, 0), "") == 0);
// assert(strcmp(ft_substr("hola", 4, 1), "") == 0);
// assert(strcmp(ft_substr("hola", 4, 20), "") == 0);
// assert(strcmp(ft_substr("hola", 5, 2), "") == 0);

// // Tests Lorem ipsum
// char	str[] = "lorem ipsum dolor sit amet";
// char	*strsub;

// strsub = ft_substr(str, 0, 10);
// assert(strcmp(strsub, "lorem ipsu") == 0);

// printf ("All tests passed");

// 	return (0);
// }
