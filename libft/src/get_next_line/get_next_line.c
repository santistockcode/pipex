/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:29:53 by saalarco          #+#    #+#             */
/*   Updated: 2025/01/09 12:41:59 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
Function used by extract line function.
This function measures string from start to n character;
returns -1 in case of error but this it's not used in the project.
*/

int	strlen_n(char *b_aux)
{
	int		n_pos;

	n_pos = 0;
	if (!b_aux)
		return (-1);
	while (b_aux[n_pos] && b_aux[n_pos] != '\n')
	{
		n_pos++;
	}
	if (b_aux[n_pos] == '\n')
		return (n_pos);
	return (-1);
}

/*
This function evaluates if buffer has '\n' or doesn't.
Then act based on that info. If no n character, clears the buffer
(*bff = 0). ft_strchr returns NULL if buffer is NULL. 
*/

char	*update_buffer(char *bff)
{
	int	fin_bff_length;

	if (ft_strchr(bff, '\n'))
	{
		fin_bff_length = ft_strlen_protected(ft_strchr(bff, '\n'));
		ft_memmove(bff, ft_strchr(bff, '\n') + 1, fin_bff_length);
		bff[fin_bff_length] = 0;
	}
	else
	{
		*bff = 0;
	}
	return (bff);
}

/*
This functions evaluates if buffer is empty, has '\n' or doesn't and
move a line based on those evalutaions.
Example: line = ABn where n is n character
strnlen_n = 2;
malloc 2(strnlen_n) + 2 (_ _ _ _)
ft_memmove for 2 + 1 (a b n _)
line[(strnlen_n) + 1](a b n 0)
final line: (a b n 0)
*/

char	*extract_line(char *buffer)
{
	char	*line;

	if (!buffer)
	{
		return (NULL);
	}
	if (ft_strchr(buffer, '\n'))
	{
		line = malloc(strlen_n(buffer) + 2);
		if (!line)
		{
			free(buffer);
			return (free(line), buffer = NULL, line = NULL, NULL);
		}
		ft_memmove(line, buffer, strlen_n(buffer) + 1);
		line[strlen_n(buffer) + 1] = '\0';
	}
	else
	{
		line = malloc(ft_strlen_protected(buffer) + 1);
		ft_memmove(line, buffer, ft_strlen_protected(buffer) + 1);
	}
	return (line);
}

/*
This function checks if static is already
allocated and allocates memory if not. Then reads
again and again from fd into that static variable
until finding n character. Then returns a pointer
to the start of the static variable.
Returns NULL in case of error.
¿why use temp: 
Memory Leak Risk: 
The original memory pointed to by left is not freed by strjoin. 
After the assignment, 
left points to the new concatenated string, 
and the original memory is lost (a memory leak occurs).
*/

static char	*read_one_line(int fd, char *left)
{
	int		bytes_read;
	char	*buff;
	char	*temp;

	buff = ft_gnl_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buff)
		return (free (buff), NULL);
	bytes_read = 1;
	while (!ft_strchr(buff, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buff);
			free(left);
			return (buff = NULL, left = NULL, NULL);
		}
		buff[bytes_read] = 0;
		temp = ft_strjoin(left, buff);
		free(left);
		left = temp;
	}
	free(buff);
	buff = NULL;
	return (left);
}

/*
This get_next_line function reads the next line from a given file descriptor fd.
It maintains a static array buffers to keep track of residual 
data for each file descriptor, allowing it to handle 
multiple file descriptors simultaneously.
*/

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buffers[MAX_FDS];

	if (fd < 0 || fd >= MAX_FDS || BUFFER_SIZE <= 0)
		return (NULL);
	buffers[fd] = read_one_line(fd, buffers[fd]);
	if (!(buffers[fd]) || *(buffers[fd]) == '\0')
	{
		free(buffers[fd]);
		buffers[fd] = NULL;
		return (NULL);
	}
	line = extract_line(buffers[fd]);
	if (!line || *line == '\0')
	{
		free(line);
		line = NULL;
		return (NULL);
	}
	else
		buffers[fd] = update_buffer(buffers[fd]);
	return (line);
}
