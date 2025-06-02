/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_count.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:28:06 by saalarco          #+#    #+#             */
/*   Updated: 2024/08/18 14:21:47 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr_count(int s, int *i)
{
	char	c;

	if (s == INT_MIN)
	{
		ft_putstr_count("-2147483648", i);
		return ;
	}
	else if (s < 0)
	{
		s = -s;
		ft_putchar_count('-', i);
	}
	if (s >= 10)
		ft_putnbr_count((s / 10), i);
	c = (s % 10) + 48;
	ft_putchar_count(c, i);
}
