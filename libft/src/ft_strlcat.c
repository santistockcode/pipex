/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 13:56:09 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 10:41:33 by saalarco         ###   ########.fr       */
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

// printf("AQUI %zu", dstlen);
// printf("AQUI %zu", dstlen);

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	srcl;
	size_t	dstlen;
	size_t	count;

	dstlen = 0;
	if (dst)
		dstlen = ft_strlen (dst);
	srcl = ft_strlen (src);
	count = dstlen;
	if (dstsize <= dstlen)
		return (dstsize + srcl);
	while ((count < (dstsize - 1)) && (*src))
	{
		*(dst + count) = *src;
		count++;
		src++;
	}
	*(dst + count) = '\0';
	return (dstlen + srcl);
}

/// versión que sí que pasa paco
// size_t	ft_strlcat(char *dest, const char *src, size_t size)
// {
// 	size_t	i;
// 	size_t	dsize;
//
// 	dsize = 0;
// 	while (dest[dsize] != '\0' && dsize < size) 
// 		dsize++;
// 	i = dsize;
// 	while (src[dsize - i] && dsize + 1 < size)
// 	{
// 		dest[dsize] = src[dsize - i];
// 		dsize++;
// 	}
// 	if (i < size)
// 		dest[dsize] = '\0';
// 	return (i + ft_strlen(src));
// }

/// versión que no pasa paco pero es clara
// size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
// {
// 	size_t	srclen;
// 	size_t	dstlen;
// 	size_t	count;
// 	count = ft_strlen (dst);
// 	srclen = ft_strlen (src);
// 	if (dstsize == 0)
// 		return (srclen);
// 	if (count >= dstsize)
// 		return (dstsize + srclen);
// 	dstlen = ft_strlen (dst);
// 	dst += 1;
// 	while (count < dstsize -1 && *src != '\0')
// 	{
// 		*dst++ = *src++;
// 		count++;
// 	}
// 	*dst = '\0';
// 	return (srclen + dstlen);
// }

/// APUNTES PROPIOS
// IMPORTANTE si truncada entonces ya no dstlen + srclen
/// if dstlen + 1 >= dstsize or dstsize = 0 return sin '\0'
/// dstsize debe ser el total del contenido  que queremos formar + 1
/// si queremos meter '\0'
/// devuelve srclen + dstlen
/// 

/// MAIN
// int main()
// {
// 		char	*dest;
// 		dest = (char *)malloc(sizeof(*dest) * 15);
// 		memset(dest, 0, 15);
// 		memset(dest, 'r', 15);
// 		printf("%zu",ft_strlcat(dest, "lorem ipsum dolor sit amet", 5));
// 		write(1, "\n", 1);
// 		write(1, dest, 15);
// }
