/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:59:57 by saalarco          #+#    #+#             */
/*   Updated: 2025/01/31 08:17:59 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_FILE_H
# define GET_FILE_H

# include "ft_printf.h"
# include "get_next_line.h"
# include "libft.h"
# include "lists.h"
# include <fcntl.h>
# include <stdlib.h>

typedef struct s_line
{
	char	*text;
	size_t	length;
}			t_line;

typedef struct s_file
{
	size_t	line_count;
	t_line	*lines;
	char	*buffer;
}			t_file;

t_list		*ft_file_to_list(int fd);
t_line		*ft_list_to_line_array(t_list *lst, size_t line_count);
char		*ft_list_to_buffer(t_list *lst);
char		**ft_line_array_to_map(t_line *lines, size_t line_count);
t_file		*ft_file_read_all(int fd);
void		ft_file_free(t_file **file);
void		ft_node_text_free(void *line);
// utility
size_t		ft_line_count(t_list *lst);
t_line		*ft_get_line_at(t_list *lst, size_t index);
const char	*ft_file_get_line(t_file *file, size_t index);
// debugging
void		ft_file_print_lines(t_file *file);
void		ft_file_print_buffer(t_file *file);
void		ft_file_print_map(t_file *file);

#endif
