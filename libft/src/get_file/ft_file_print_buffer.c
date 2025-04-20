/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_print_buffer.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 08:28:03 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 11:20:55 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

void	ft_file_print_buffer(t_file *file)
{
	ft_printf("ft_file_print_buffer:\n%s\n", file->buffer);
}
