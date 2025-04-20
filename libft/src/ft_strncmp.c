/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 12:47:30 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/12 12:47:32 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	c;
	int		ans;

	c = 0;
	ans = 0;
	while (c < n && (s1[c] != '\0' || s2[c] != '\0'))
	{
		if (s1[c] != s2[c])
			return ((unsigned char)s1[c] - (unsigned char) s2[c]);
		c++;
	}
	return (ans);
}
/*
antes: while ((c < n) && (*s1 == *s2)...
Con este paréntesis si n es 0 si que es okey para el condicional, 
cortocircuita y hace 10 vueltas,
solución: no poner paréntesis en c < n
Caso contrario, si n es 10 entonces nos salimos del array
y tengo un resultado errático si lo comparo ahí
necesita de if (c < n), comprobar si no hemos llegado al final
y son las dos cadenas iguales 
*/
