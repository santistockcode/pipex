/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_get_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 20:00:52 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 11:20:51 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

const char	*ft_file_get_line(t_file *file, size_t index)
{
	if (file == NULL || file->lines == NULL)
		return (NULL);
	if (index < file->line_count)
		return (file->lines[index].text);
	else
		return (NULL);
}
