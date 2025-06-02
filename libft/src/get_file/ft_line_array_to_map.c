/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_array_to_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 18:19:12 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 11:21:19 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

static void	free_all(char **chars_matrix, size_t index)
{
	while (index > 0)
	{
		index--;
		free(chars_matrix[index]);
	}
	free(chars_matrix);
}

size_t	calc_line_map_length(t_line *lines_array, size_t line_count,
		size_t i)
{
	if ((i < line_count - 1)
		|| lines_array[i].text[lines_array[i].length - 1] == '\n')
		return (lines_array[i].length - 1);
	else
		return (lines_array[i].length);
}

/*
It takes an array of t_line lines.
Removes the trailing newline character if present.
Returns a char ** where each element is a row of characters without \n.
It does NOT null-terminate the strings.
On failure, returns NULL and frees all allocated memory.
*/
char	**ft_line_array_to_map(t_line *lines_array, size_t line_count)
{
	char	**chars_matrix;
	char	*map_row;
	size_t	i;
	size_t	line_map_length;

	if (line_count == 0 || lines_array == NULL)
		return (NULL);
	chars_matrix = (char **)malloc(line_count * sizeof(char *));
	if (chars_matrix == NULL)
		return (NULL);
	i = 0;
	while (i < line_count)
	{
		line_map_length = calc_line_map_length(lines_array, line_count, i);
		map_row = (char *)malloc(line_map_length * sizeof(char));
		if (map_row == NULL)
		{
			free_all(chars_matrix, i);
			return (NULL);
		}
		ft_memcpy(map_row, lines_array[i].text, line_map_length);
		chars_matrix[i] = map_row;
		i++;
	}
	return (chars_matrix);
}
