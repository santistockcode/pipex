/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 09:42:21 by saalarco          #+#    #+#             */
/*   Updated: 2024/02/14 09:55:57 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// # include <stddef.h>
// # include <stdlib.h>
// # include <limits.h>
// # include <unistd.h>
// # include <stdio.h>

// cc ft_strjoin.c ft_strlen.c ft_strlcpy.c ft_strlcat.c

char	*ft_strjoin(char const *s1, char const *s2)
{
	char				*resstr;
	unsigned int		s1len;
	unsigned int		s2len;

	s1len = 0;
	s2len = 0;
	if (s1)
		s1len = ft_strlen(s1);
	if (s2)
		s2len = ft_strlen(s2);
	resstr = (char *)malloc((s1len + s2len + 1) * sizeof(char));
	if (resstr != NULL)
	{
		ft_strlcpy(resstr, s1, s1len + 1);
		ft_strlcat(resstr, s2, s1len + s2len + 1);
	}
	else
		return (NULL);
	return (resstr);
}

/*
Descripción:
Reserva (con malloc(3)) y devuelve una nueva
string, formada por la concatenación de ’s1’ y
’s2’.
*/

/*
Valor devuelto: 
Reserva (con malloc(3)) y devuelve una nueva
string, formada por la concatenación de ’s1’ y
’s2’.
*/

/*
Parámetros:
s1: La primera string.
s2: La string a añadir a ’s1’
*/

// Test function for ft_strjoin

// void test_ft_strjoin()
// {
//     const char *s1 = "Hello, ";
//     const char *s2 = "World!";
//     printf("String 1: %s\n", s1);
//     printf("String 2: %s\n", s2);
//     // Concatenate the strings using ft_strjoin
//     char *result = ft_strjoin(s1, s2);
//     printf("Concatenated string: %s\n", result);
//     // Free the allocated memory
//     free(result);
// }

// int main()
// {
//     test_ft_strjoin();
//     return 0;
// }
