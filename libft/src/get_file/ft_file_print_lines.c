/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_print_lines.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 08:27:48 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 11:21:00 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

void	ft_file_print_lines(t_file *file)
{
	size_t	index;

	index = 0;
	ft_printf("ft_file_print_lines:\n");
	if (file->line_count == 0)
		return ;
	while (index < file->line_count)
	{
		ft_printf("%s", (file->lines)[index].text);
		index++;
	}
	ft_printf("\n");
}
