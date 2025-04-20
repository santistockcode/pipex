/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_to_line_array.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:47:21 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 11:21:33 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

static void	*free_line_array_text(t_line *line_array, size_t index)
{
	while (index > 0)
	{
		index--;
		free(line_array[index].text);
	}
	free(line_array);
	return (NULL);
}

t_line	*ft_list_to_line_array(t_list *lst, size_t line_count)
{
	size_t	index;
	t_line	*line_array;
	t_line	src;

	index = 0;
	if (line_count == 0 || lst == NULL)
		return (NULL);
	line_array = (t_line *)malloc(line_count * sizeof(t_line));
	if (line_array == NULL)
		return (NULL);
	while (index < line_count)
	{
		src = *(t_line *)lst->content;
		line_array[index].text = ft_strdup(src.text);
		if (line_array[index].text == NULL)
			return (free_line_array_text(line_array, index), NULL);
		line_array[index].length = src.length;
		lst = lst->next;
		index++;
	}
	return (line_array);
}
