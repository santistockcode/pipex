/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_to_buffer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 18:37:04 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 11:21:28 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

size_t	measure_total_length(t_list *lst)
{
	size_t	total_len;

	total_len = 0;
	while (lst != NULL)
	{
		total_len += ((t_line *)(lst->content))->length;
		lst = lst->next;
	}
	return (total_len);
}

/*
Iterates over the list of t_line elements.
Calculates total size needed (sum of lengths + possible newline characters).
Allocates and copies all lines into one continuous char array.
Returns the buffer (caller must free).
*/
char	*ft_list_to_buffer(t_list *lst)
{
	char	*buffer;
	size_t	total_len;

	total_len = 0;
	total_len = measure_total_length(lst);
	buffer = (char *)malloc(total_len + 1);
	if (buffer == NULL)
		return (NULL);
	buffer[0] = '\0';
	while (lst != NULL)
	{
		ft_strlcat(buffer, ((t_line *)(lst->content))->text, total_len + 1);
		lst = lst->next;
	}
	return (buffer);
}
