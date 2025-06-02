/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_node_text_free.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:58:35 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 11:22:42 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

void	ft_node_text_free(void *line)
{
	t_line	*line_struct;

	line_struct = (t_line *)line;
	free(line_struct->text);
	free(line_struct);
}
