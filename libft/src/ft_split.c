/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:39:21 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/30 11:39:25 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// #include <assert.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <ctype.h>
// #include <limits.h>
// #include <string.h>
// #include <stdlib.h>

/*
Calcula el número de caracteres hasta el  delimitador
*/
static size_t	ft_custom_strlen(const char *s, char chr)
{
	size_t	c;

	c = 0;
	while (s && *s != '\0' && *s++ != chr)
		c++;
	return (c);
}

/*
Cuenta las palabras en la cadena delimitadas por del
*/
static size_t	ft_count_words(char *str, char del)
{
	size_t	str_iter;
	size_t	words_c;

	str_iter = 0;
	words_c = 0;
	if (str == NULL || *str == '\0')
		return (0);
	else if (del == '\0')
		return (1);
	else
	{
		while (str[str_iter] != '\0')
		{
			if (str[str_iter] != del)
			{
				words_c++;
				while (str[str_iter] != del && str[str_iter] != '\0')
					str_iter++;
			}
			while (str[str_iter] == del && str[str_iter] != '\0')
				str_iter++;
		}
	}
	return (words_c);
}

/*
Libera la memoria de las anteriores posiciones del array
Aquí es necesario hacer --iter para evitar entrar
en una posición que no controlamos.
*/
static char	**free_previous(char **strings_array, size_t iter)
{
	while (iter > 0)
	{
		free(strings_array[--iter]);
	}
	free(strings_array);
	return (NULL);
}

/*
copia en el string str_dst posición del array
la siguiente palabra str_src hasta delim.
*/
static char	*ft_strlcpy_from_to(char *str_src, size_t n_words,
size_t words_iter, char delim)
{
	if (words_iter < n_words - 1)
	{
		str_src += ft_custom_strlen(str_src, delim);
		while (str_src && *str_src == delim)
			str_src++;
	}
	return (str_src);
}

char	**ft_split(char const *s, char c)
{
	char	*str;
	size_t	n_words;
	size_t	w_iter;
	char	**s_res;

	str = (char *)s;
	while (c != 0 && str && *str && *str == c)
		str++;
	n_words = ft_count_words(str, c);
	s_res = (char **) malloc ((n_words + 1) * sizeof(char *));
	if (s_res == NULL)
		return (NULL);
	w_iter = 0;
	while (w_iter < n_words)
	{
		s_res[w_iter] = (char *)
			malloc ((ft_custom_strlen(str, c) + 1) * sizeof(char));
		if (s_res[w_iter] == NULL)
			return (free_previous(s_res, w_iter));
		ft_strlcpy(s_res[w_iter], str, (ft_custom_strlen(str, c)) + 1);
		str = ft_strlcpy_from_to(str, n_words, w_iter, c);
		w_iter++;
	}
	s_res[n_words] = (void *) 0;
	return (s_res);
}

// int main()
// {
// 	// char *check = 0;
// 	// char *str_trimmed = ft_strtrim((char *)check, 0);

// 	char *cadena = "uno dos    tres   cuatro   ";
// 	char **res = ft_split(cadena, ' ');
// 	assert(strcmp(res[0], "uno") == 0);
// 	assert(strcmp(res[1], "dos") == 0);
// 	assert(strcmp(res[2], "tres") == 0);
// 	assert(strcmp(res[3], "cuatro") == 0);
// 	assert(res[4] == NULL);

// 	//Preparar
// 	char *cadena1 = "saalarco";
// 	//Actuar
// 	char **res1 = ft_split(cadena1, ' ');
// 	//Testear
// 	assert(strcmp(res1[0], "saalarco") == 0);
// 	assert(res1[1] == NULL);	

// 	//Preparar
// 	char *cadena2 = "saalarco";
// 	//Actuar
// 	char **res2 = ft_split(cadena2, 0);
// 	//Testear
// 	assert(strcmp(res2[0], "saalarco") == 0);
// 	assert(res2[1] == NULL);

// 	//Preparar
// 	char *cadena3 = 0;
// 	//Actuar
// 	char **res3 = ft_split(cadena3, 0);
// 	//Testear
// 	assert(res3[0] == NULL);

// 	//Preparar
// 	char *cadena4 = "^^^1^^2a,^^^^3^^^^--h^^^^";
// 	//Actuar
// 	char **res4 = ft_split(cadena4, '^');
// 	// //Testear
// 	assert(strcmp(res4[0], "1") == 0);
// 	assert(strcmp(res4[1], "2a,") == 0);
// 	assert(strcmp(res4[2], "3") == 0);
// 	assert(strcmp(res4[3], "--h") == 0);

// 	printf("Test passed!\n");
// }