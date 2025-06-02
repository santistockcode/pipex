/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 18:05:20 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/14 18:05:23 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			count;
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	ptr1 = (unsigned char *) s1;
	ptr2 = (unsigned char *) s2;
	count = 0;
	if (n == 0)
		return (0);
	while (count < n)
	{
		if (*ptr1 != *ptr2)
		{
			return (*ptr1 - *ptr2);
		}
		else
		{
			ptr1++;
			ptr2++;
			count++;
		}
	}
	return (0);
}
/*
Si son identicos o n es 0 devuelve 0
Lo que se compara es el contenido de los bytes equiparado 
a unsigned char 
*/
