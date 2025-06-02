/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:09:24 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 10:22:30 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t			cntr;
	unsigned char	*pntr;

	cntr = 0;
	pntr = s;
	if (n <= 0)
		return ;
	while (cntr < n)
	{
		pntr[cntr] = 0;
		cntr++;
	}
}
