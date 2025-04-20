/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:13:40 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/26 11:13:43 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// #include "string.h"
// #include "stdlib.h"
// #include "stdio.h"

// Función que devuelve true o false si el character está en el set
int	ft_is_in_set(char c, char *set)
{
	char	*set_iter;

	set_iter = set;
	while (*set_iter)
	{
		if (*set_iter == c)
			return (1);
		set_iter++;
	}
	return (0);
}

size_t	calc_malloc_len(size_t malloc_len, char *suff_s1,
size_t prefix_len, char const *set)
{
	size_t	sufix_len;

	sufix_len = 0;
	while (sufix_len < malloc_len && ft_is_in_set(*suff_s1--, (char *) set))
	{
		sufix_len++;
	}
	if (malloc_len < (prefix_len + sufix_len))
		malloc_len = 0;
	else
		malloc_len = malloc_len - prefix_len - sufix_len;
	return (malloc_len);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	malloc_len;
	size_t	prefix_len;
	char	*pref_s1;
	char	*suff_s1;
	char	*response_str;

	if (s1 == 0)
		malloc_len = 0;
	else
		malloc_len = ft_strlen(s1);
	suff_s1 = (char *)(s1 + ft_strlen(s1) - 1);
	pref_s1 = (char *)s1;
	prefix_len = 0;
	while (ft_is_in_set(*s1++, (char *)set))
	{
		prefix_len++;
	}
	malloc_len = calc_malloc_len(malloc_len, suff_s1, prefix_len, set);
	response_str = (char *) malloc ((malloc_len + 1) * sizeof(char));
	if (response_str == NULL)
		return (NULL);
	ft_strlcpy (response_str, pref_s1 + prefix_len, (malloc_len + 1));
	return (response_str);
}
/// CÓDIGO COMENTADO
// char    *ft_strtrim(char const *s1, char const *set)
// {
// 	// size for malloc
// 	size_t	mallocLen;
// 	// size de prefix
// 	size_t	prefixLen;
// 	// size de post
// 	size_t	sufixLen;
// 	// cadena de paso
// 	char	*pref_s1;
// 	// cadena de paso
// 	char 	*suff_s1;
// 	// resultante
// 	char 	*responseStr;
// 	// original (para luego optimizar)
// 	// char *originals1;

// 	if (s1 == '\0')
// 		mallocLen = 0;
// 	else
// 		mallocLen = ft_strlen(s1);
// 	suff_s1 = (char *)(s1 + ft_strlen(s1) - 1); 
// 	pref_s1 = (char *)s1;
// 	// originals1 = (char *) s1;
// 	// x.x.x.x.0.0.0.0.0.x.x.x  EXAMPLE
// 	prefixLen = 0;
// 	// si no existen set x size = strlen s1
// 	// si no existe s1 pues size = strlen s1 (0)
// 	// if (*set == '\0')
// 	// 	mallocLen = 0;
// 	// Calcular prefix while char in set avanza prefix (4)
// 	while (ft_is_in_set(*s1++, (char *)set))
// 	{
// 		prefixLen++;
// 	}
// 	// printf("PREFIX:%zu", prefixLen);
// 	// Calcular postfix (3)
// 	// por cada caracter de set cuento trim final
// 	sufixLen = 0;
// 	while (sufixLen < mallocLen && ft_is_in_set(*suff_s1--, (char *) set))
// 	{
// 		sufixLen++;
// 	}
// 	// printf("SUFF:%zu", sufixLen);
// 	// malloc adecuado 
// 		// si no se puede retorna NULL
// 	if(mallocLen < (prefixLen  + sufixLen))
// 		mallocLen = 0;
// 	else
// 		mallocLen = mallocLen - prefixLen - sufixLen;
// 	// printf("MALLLOCLEN:%zu", mallocLen);
// 	responseStr = malloc ((mallocLen + 1) * sizeof(char));
// 	if (responseStr == NULL)
// 		return (NULL);

// 	// strlcpy desde donde sea hasta donde sea
// 	// strlcpy(resstr, s1 + prefix, ft_strlen(s1) - prefix - posix )
// 	// s1 es donde se haya quedado s1
// 	ft_strlcpy(responseStr, pref_s1 + prefixLen , (mallocLen + 1)); 
// 	// printf("RESPONSE:%s", responseStr);
// 	// return resultante
// 	// size_t i;
// 	// s1 = "hola";
// 	// i = ft_strlen(s1);
// 	// i++;
// 	// set = ft_itoa((int)i);
// 	// i = ft_strlen(set);
// 	return (responseStr);
// }

// int main()
// {
// 	char *cadena = "ddd123aaaaaaaa";
// 	char *set = "amanda";
// 	ft_strtrim(cadena, set);
// 	return (0);
// }

/*
Parámetros
s1: La string que debe ser recortada.
set: Los caracteres a eliminar de la string.
*/

/*
Retorna
La string recortada.
NULL si falla la reserva de memoria.
*/

/*
Descripción
Elimina todos los caracteres de la string ’set’
desde el principio y desde el final de ’s1’, hasta
encontrar un caracter no perteneciente a ’set’. La
string resultante se devuelve con una reserva de
malloc(3
*/
