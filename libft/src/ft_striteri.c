/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strteri.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:28:53 by saalarco          #+#    #+#             */
/*   Updated: 2024/02/01 17:28:55 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// # include <stddef.h>
// # include <stdlib.h>
// # include <limits.h>
// # include <unistd.h>
// # include <stdio.h>

// void example(unsigned int index, char *c)
// {
//     if (index % 2 == 0)
//         *c = *c - 32;
//     else
//         *c = 'a';
// }

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	index_in_the_string;
	char			*where_we_at;

	index_in_the_string = 0;
	where_we_at = s;
	while (*where_we_at)
	{
		f (index_in_the_string, where_we_at);
		where_we_at++;
		index_in_the_string++;
	}
}

// void test_ft_striteri()
// {
//     char str[] = "holamundo";
//     printf("Original string: %s\n", str);
//     // Apply ft_striteri to modify the string
//     ft_striteri(str, example);
//     printf("Modified string: %s\n", str);
// }

// int main()
// {
//     test_ft_striteri();
//     return 0;
// }
