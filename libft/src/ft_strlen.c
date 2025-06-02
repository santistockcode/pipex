/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 12:34:43 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/09 19:39:12 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// #include <stdio.h>
// #include <string.h>

size_t	ft_strlen(const char *s)
{
	size_t	c;

	c = 0;
	while (*s++ != '\0')
		c++;
	return (c);
}

/*
int main (void)
{
	char *s;

	s = NULL;

	ft_putnbr_fd(ft_strlen(s), 1);
}

/// a posteriori: proteger *s
*/
