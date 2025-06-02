/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 17:29:43 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/12 17:29:45 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	paso;

	if (*s == '\0' && c == '\0')
		return ((char *)s);
	paso = (char) c;
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

// int main()
// {
// char *d3;
// char *d4;
// char *buff[1024];
// bzero(buff, 1024);
//     if (!(d3= strchr("test", buff)))
//         puts("original NULL");
//     else
//     {
//         puts("here it is d3");
//         puts(d3); 
//         if(d3 != ("test" + 4))
//             puts("original Return value is false");
//         else (puts("original Return value is true"));
//     }
//     if(!(d4 = ft_strchr("test", buff)))
//         puts("propia NULL");
//     else
//     {
//         puts("here it is d4");
//         puts(d4); 
//         if(d4 != ("test" + 4))
//             puts("propia Return value is false");
//         else (puts("propia Return value is true"));
//     }
// }