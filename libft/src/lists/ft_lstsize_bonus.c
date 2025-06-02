/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:42:16 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/06 16:32:09 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lists.h"

// #include <assert.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <ctype.h>
// #include <limits.h>
// #include <string.h>
// #include <stdlib.h>
// ft_lstnew
// ft_lstadd_front

// typedef struct s_list
// {
// 	void			*content;
// 	struct s_list	*next;
// }	t_list;

int	ft_lstsize(t_list *lst)
{
	int		length_node;
	t_list	*tmp_node;

	length_node = 0;
	if (!lst)
		return (0);
	tmp_node = lst;
	while (tmp_node != NULL)
	{
		length_node++;
		tmp_node = tmp_node->next;
	}
	return (length_node);
}

// int main()
// {
// 	t_list* list = NULL;

// 	t_list* nodo1 = ft_lstnew("nodo1");
// 	t_list* nodo2 = ft_lstnew("nodo2");
// 	t_list* nodo3 = ft_lstnew("nodo3");

// 	ft_lstadd_front(&list, nodo3);
// 	ft_lstadd_front(&list, nodo2);
// 	ft_lstadd_front(&list, nodo1);

// 	int list_length = ft_lstsize(list);

// 	printf("Size of list: %d", list_length);

// 	t_list *current = list;

// 	while (current != NULL)
// 	{
// 		t_list *tmp = current->next;
// 		free(current);
// 		current = tmp;
// 	}
// 	return (0);
// }
