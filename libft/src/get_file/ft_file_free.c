/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:28:33 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 11:20:48 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

/*
 * Frees all memory associated with `*file`:
 * - Frees each `lines[i].text`
 * - Frees `lines`
 * - Frees `buffer`
 * - Frees the `t_file` itself
 * Sets `*file = NULL`.
 */
void	ft_file_free(t_file **file)
{
	size_t	index;
	t_file	*file_ref;

	index = 0;
	file_ref = *file;
	if (file_ref->lines)
	{
		while (index < file_ref->line_count)
		{
			free(file_ref->lines[index].text);
			index++;
		}
		free(file_ref->lines);
	}
	free(file_ref->buffer);
	free(file_ref);
	*file = NULL;
}
