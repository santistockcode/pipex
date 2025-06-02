/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 16:49:06 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 10:41:21 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*ptr;
	size_t	s_len;

	s_len = ft_strlen(s1) + 1;
	ptr = malloc(s_len * sizeof(char));
	if (ptr == NULL)
		return (NULL);
	ft_memcpy(ptr, s1, s_len);
	return (ptr);
}

// // manejo de memoria para strings
// char	*ft_strdup(const char *s1)
// {
// 	char	*ptr;
// 	size_t	s_len;

// 	s_len = ft_strlen(s1) + 1;
// 	// malloc reserva memoria
// 	ptr = malloc(s_len * sizeof(char));
// 	// si no hay memoria disponible, malloc devuelve NULL
// 	if (ptr == NULL)
// 		return (NULL);
// 	// copia el contenido de s1 en ptr
// 	ft_strlcpy(ptr, s1, s_len);
// 	return (ptr);
// }
