/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:34:09 by saalarco          #+#    #+#             */
/*   Updated: 2025/01/31 08:17:42 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// ft_strchr from libft
char	*ft_strchr(const char *s, int c)
{
	char	paso;

	if (*s == '\0' && c == '\0')
		return ((char *)s);
	paso = (char)c;
	while (*s != paso && *s)
	{
		s++;
	}
	if (*s == paso)
	{
		return ((char *)s);
	}
	else
	{
		return (NULL);
	}
}

// ft_strjoin from libft but checking NULL str1 str2
char	*ft_strjoin(char const *str1, char const *str2)
{
	char	*r_join;
	int		i;

	i = 0;
	if (str1 == NULL && str2 == NULL)
		return (NULL);
	r_join = (char *)malloc((ft_strlen_protected(str1)
				+ ft_strlen_protected(str2) + 1) * sizeof(char));
	if (r_join == NULL)
		return (NULL);
	if (str1)
	{
		while (*str1)
			r_join[i++] = *str1++;
	}
	while (*str2)
		r_join[i++] = *str2++;
	r_join[i] = '\0';
	return (r_join);
}

// ft_strlen but checks for !s
size_t	ft_strlen_protected(const char *s)
{
	size_t	c;

	c = 0;
	if (!s)
		return (0);
	while (*s++ != '\0')
		c++;
	return (c);
}

/*
This function is like strcpy but with memory
so we differenciate copy left vs copy right since they might overlap
i is for index
*/
void	*ft_memmove(void *destination, const void *source, size_t len)
{
	char	*src;
	char	*dest;
	size_t	i;

	src = (char *)source;
	dest = (char *)destination;
	i = 0;
	if (dest > src)
	{
		while (len > 0)
		{
			dest[len - 1] = src[len - 1];
			len--;
		}
	}
	else
	{
		while (i < len)
		{
			dest[i] = src[i];
			i++;
		}
	}
	return (destination);
}

// calloc from libft
void	*ft_gnl_calloc(size_t count, size_t size)
{
	void	*result;
	size_t	i;
	char	*mem;

	result = malloc(count * size);
	if (result == NULL)
		return (NULL);
	mem = result;
	i = 0;
	while (i < count * size)
	{
		mem[i] = '\0';
		i++;
	}
	return (result);
}
