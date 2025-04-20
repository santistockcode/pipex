/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:35:36 by saalarco          #+#    #+#             */
/*   Updated: 2024/02/01 17:35:38 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// # include <stddef.h>
// # include <stdlib.h>
// # include <limits.h>
// # include <unistd.h>
// # include <stdio.h>

// size_t	ft_strlen(const char *s)
// {
// 	size_t	c;

// 	c = 0;
// 	while (*s++ != '\0')
// 		c++;
// 	return (c);
// }

// char example (unsigned int i, char c)
// {
// 	if (i % 2 == 0)
// 		c = c - 32;
// 	return c;
// }

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	index_in_the_string;
	char			*where_we_at;
	char			*response_string;
	size_t			size_for_malloc;

	index_in_the_string = 0;
	where_we_at = (char *) s;
	size_for_malloc = ft_strlen((char *) s);
	response_string = (char *) malloc ((size_for_malloc + 1) * sizeof(char));
	if (response_string == NULL)
		return (NULL);
	while (*where_we_at)
	{
		*response_string = f (index_in_the_string, (char) *where_we_at);
		where_we_at++;
		index_in_the_string++;
		response_string++;
	}
	*response_string = '\0';
	response_string = response_string - index_in_the_string;
	return (response_string);
}

// void test_ft_strmapi()
// {
//     char str[] = "holamundo";
//     printf("Original string: %s\n", str);
//     // Apply ft_strmapi to create a new modified string
//     char *new_str = ft_strmapi(str, example);

// 	if (new_str == str)
// 		printf("Nada nuevo");
//     printf("Modified string: %s\n", new_str);
//     free(new_str);
// }

// int main(void)
// {
// 	test_ft_strmapi();
// 	return (0);
// }