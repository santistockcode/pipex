/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putbase_count.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:49:33 by saalarco          #+#    #+#             */
/*   Updated: 2024/08/18 14:23:29 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr_base_count(size_t n, char *base, size_t len, int *i)
{
	if (n >= len)
		ft_putnbr_base_count((n / len), base, len, i);
	ft_putchar_count(base[(n % len)], i);
}

void	ft_putbase_count(size_t nbr, char *base, int *i, int pointer)
{
	if (nbr == 0 && pointer == 1)
	{
		*i += write(1, "(nil)", 5);
		return ;
	}
	if (pointer == 1)
	{
		*i += write(1, "0x", 2);
		ft_putnbr_base_count(nbr, base, ft_strlen(base), i);
	}
	if (pointer == 0)
		ft_putnbr_base_count(nbr, base, ft_strlen(base), i);
}
