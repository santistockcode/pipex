/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 17:04:21 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/16 17:04:26 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	move_right(unsigned char *b, unsigned char *o, size_t l)
{
	while (l > 0)
	{
		b[l - 1] = o[l - 1];
		l--;
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*bufer;
	unsigned char	*origen;
	int				c;

	bufer = (unsigned char *) dst;
	origen = (unsigned char *) src;
	c = 0;
	if (!dst && !src)
		return (0);
	if (dst <= src)
	{
		while (c < (int) len)
		{
			bufer[c] = origen[c];
			c++;
		}
	}
	else if (dst > src)
	{
		move_right(bufer, origen, len);
	}
	return (dst);
}

/*
Ejemplo:
Copiar 5 caracteres desde el número 3 y colocarlos a partir del 7:
De 123456789123456789 se pasa 34567 12[34567]89123456789 a donde está el 7:
   123456345673456789
En este caso se hace uso de void move_right, esto es,
se empieza a copiar el 7 hacia el 3 para no perder información.
Si se hace al revés, se pierde información.
*/