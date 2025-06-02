/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:38:03 by saalarco          #+#    #+#             */
/*   Updated: 2025/01/09 12:42:31 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 3
# endif

# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>

# define MAX_FDS 1024

char	*get_next_line(int fd);
void	*ft_gnl_calloc(size_t count, size_t size);
void	*ft_memmove(void *dst, const void *src, size_t len);
size_t	ft_strlen_protected(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);

#endif
