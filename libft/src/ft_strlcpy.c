/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 17:52:43 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/23 19:29:00 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	lens;
	size_t	count;

	lens = (size_t) ft_strlen(src);
	if (dstsize == 0)
		return (lens);
	count = 0;
	while (count + 1 < dstsize && *src)
	{
		*dst++ = *src++;
		count++;
	}
	if (count < dstsize)
	{
		*dst = '\0';
	}
	return (lens);
}

// size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
// {
// 	size_t	i;

// 	i = 0;
// 	if (dstsize == 0)
// 	{
// 		while (src[i])
// 			i++;
// 		return (i);
// 	}
// 	while (i < dstsize - 1 && src[i] != '\0')
// 	{
// 		dst[i] = src[i];
// 		i++;
// 	}
// 	if (i < dstsize)
// 		dst[i] = '\0';
// 	while (src[i] != '\0')
// 		i++;
// 	return (i);
// }

// size_t  ft_strlcpy(char *dst, const char *src, size_t size)
// {
//     size_t i;
//     size_t src_len;

//     if (!dst || !src)
//         return (0);
//     src_len = ft_strlen(src);
//     if (size == 0)
//         return (src_len);
//     i = 0;
//     while (src[i] && i < (size - 1))
//     {
//         dst[i] = src[i];
//         i++;
//     }
//     dst[i] = '\0';
//     return (src_len);
// }

/*
If dstsize == 0 no se añade nada ni se añade '\0'
&& *src es para que no machaque contenido si dstsize es mayor que src size
*/
//Copia src en dst, añadiendo '\0' al final
// size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
// {
// 	size_t	lens;
// 	size_t	count;

// 	lens = (size_t) ft_strlen(src);
// 	//si dstsize es 0, retorna el tamaño de src
// 	if (dstsize == 0)
// 		return (lens);
// 	count = 0;
// 	//dstsize - 1 para dejar espacio para '\0'
// 	//*src evita que ponga un '\0' e interrumpa la cadena
// 	while (count + 1 < dstsize && *src)
// 	{
// 		*dst++ = *src++;
// 		count++;
// 	}
// 	// aquí es donde se pone el '\0'
// 	if (*dst)
// 	{
// 		*dst = '\0';
// 	}
// 	return (lens);
// }
/* Si para un char dst[5] pasas dstsize 4, va a incluir:
3 caracteres y el '\0' */
/* Si para un char dst[5] pasas dstsize 6, no lo añade*/
