/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 15:47:03 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/14 15:47:06 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			count;
	unsigned char	*paso;

	paso = (unsigned char *) s;
	count = 0;
	while (count < n)
	{
		*paso++ = (unsigned char) c;
		count++;
	}
	return (s);
}

/*mínimo espacio por tipo es el de char: byte-level*/
/*size_t solo limita el que no sean negativos*/
/*puede dar problemas si se usa con ints por ejemplo*/