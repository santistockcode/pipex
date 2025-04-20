/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_to_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:12:15 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/23 11:03:21 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

void	free_line(void *line)
{
	t_line	*line_struct;

	line_struct = (t_line *)line;
	free(line_struct->text);
	free(line_struct);
}

t_line	*create_line(char *text)
{
	t_line	*line_struct;

	line_struct = (t_line *)malloc(sizeof(t_line));
	if (line_struct == NULL)
		return (NULL);
	line_struct->text = ft_strdup(text);
	if (line_struct->text == NULL)
	{
		free(line_struct);
		return (NULL);
	}
	line_struct->length = ft_strlen(line_struct->text);
	return (line_struct);
}

/*
Reads a file line-by-line using get_next_line (or similar) and stores each line
as a t_line in a t_list. Returns the head of the list, or NULL on error.
TODO: a better use of lists would be to keep the las node 
and addback to that last added node, 
that way you don't have to traverse the list every time you add a new node.
*/
t_list	*ft_file_to_list(int fd)
{
	char	*line;
	t_line	*line_struct;
	t_list	*head;
	t_list	*new_node;

	head = NULL;
	line = get_next_line(fd);
	while (line != NULL)
	{
		line_struct = create_line(line);
		if (line_struct == NULL)
		{
			return (free(line), ft_lstclear(&head, free_line), NULL);
		}
		free(line);
		new_node = ft_lstnew((t_line *)line_struct);
		if (new_node == NULL)
		{
			free_line(line_struct);
			return (ft_lstclear(&head, free_line), NULL);
		}
		ft_lstadd_back(&head, new_node);
		line = get_next_line(fd);
	}
	return (head);
}
