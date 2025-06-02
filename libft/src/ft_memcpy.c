/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:16:06 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/14 17:16:08 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			cont;
	unsigned char	*bufer;
	unsigned char	*origen;

	bufer = (unsigned char *) dst;
	origen = (unsigned char *) src;
	cont = 0;
	if (dst == src)
		return (dst);
	while (cont < n)
	{
		*bufer++ = *origen++;
		cont++;
	}
	return (dst);
}
/*
Si no controlo que origen y bufer sean igual da problema 
cuando mando dos nulls  pointers
pero sí que tiene que dar seg fault 
cuando envío null y otra cosa 
*/
