/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_read_all.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 08:18:24 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 11:21:05 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

t_file	*ft_empty_file(void)
{
	t_file	*file;

	file = (t_file *)malloc(sizeof(t_file));
	if (!file)
		return (NULL);
	ft_bzero(file, sizeof(t_file));
	file->buffer = ft_strdup("");
	if (!file->buffer)
	{
		free(file);
		return (NULL);
	}
	file->line_count = 0;
	file->lines = NULL;
	return (file);
}

void	free_list_and_file(t_list *list_lines, t_file *file)
{
	ft_lstclear(&list_lines, ft_node_text_free);
	free(file);
}

void	free_list_lines_and_file(t_list *list_lines, t_file *file)
{
	size_t	index;

	index = 0;
	ft_lstclear(&list_lines, ft_node_text_free);
	while (index < file->line_count)
	{
		free(file->lines[index].text);
		index++;
	}
	free(file->lines);
	free(file);
}

t_file	*ft_file_read_all(int fd)
{
	t_file	*file;
	t_list	*list_lines;

	if (fd < 0)
		return (NULL);
	list_lines = ft_file_to_list(fd);
	if (!list_lines)
		return (ft_empty_file());
	file = (t_file *)malloc(sizeof(t_file));
	if (!file)
		return (ft_lstclear(&list_lines, ft_node_text_free), NULL);
	ft_bzero(file, sizeof(t_file));
	file->line_count = ft_line_count(list_lines);
	file->lines = ft_list_to_line_array(list_lines, file->line_count);
	if (!(file->lines))
		return (free_list_and_file(list_lines, file), NULL);
	file->buffer = ft_list_to_buffer(list_lines);
	if (!(file->buffer))
		return (free_list_lines_and_file(list_lines, file), NULL);
	ft_lstclear(&list_lines, ft_node_text_free);
	return (file);
}
