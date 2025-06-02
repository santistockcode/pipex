/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:26:10 by saalarco          #+#    #+#             */
/*   Updated: 2024/02/02 11:26:13 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstring_fd(char *str, int fd)
{
	while (*str)
	{
		ft_putchar_fd(*str++, fd);
	}
}

void	ft_putnbr_fd(int nb, int fd)
{
	char	c;

	if (nb == INT_MIN)
	{
		ft_putstring_fd("-2147483648", fd);
		return ;
	}
	else if (nb < 0)
	{
		nb = -nb;
		ft_putchar_fd('-', fd);
	}
	if (nb >= 10)
		ft_putnbr_fd((nb / 10), fd);
	c = (nb % 10) + 48;
	ft_putchar_fd(c, fd);
}

// int	main(void)
// {
// 	int j = -1234567111;
// 	ft_putnbr_fd(j, 1);
// }
