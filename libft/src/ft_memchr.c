/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:23:27 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/14 16:23:29 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*paso;
	size_t			count;
	unsigned char	car;

	paso = (unsigned char *)s;
	count = n;
	car = (unsigned char) c;
	while (count > 0)
	{
		if (*paso != (unsigned char) car)
		{
			paso++;
			count--;
		}
		else
			return (paso);
	}
	return (NULL);
}
