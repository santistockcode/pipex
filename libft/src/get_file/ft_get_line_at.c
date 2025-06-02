/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_line_at.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 19:49:24 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 11:21:14 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_file.h"

t_line	*ft_get_line_at(t_list *lst, size_t index)
{
	size_t	i;

	i = 0;
	while (lst != NULL)
	{
		if (i == index)
			return ((t_line *)lst->content);
		lst = lst->next;
		i++;
	}
	return (NULL);
}
