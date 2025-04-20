/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:25:29 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/06 16:31:46 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lists.h"

// #include <assert.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <ctype.h>
// #include <limits.h>
// #include <string.h>
// #include <unistd.h>
// #include <stdlib.h>

// typedef struct s_list
// {
// 	void			*content;
// 	struct s_list	*next;
// }	t_list;

/*
Si falla primero aplica del y luego libera
*/

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;
	t_list	*ptr;

	ptr = NULL;
	if ((*lst) == NULL)
		return ;
	if ((*lst)->next == NULL)
	{
		del((*lst)->content);
		free(*lst);
		*lst = NULL;
		return ;
	}
	del((*lst)->content);
	ptr = (*lst)->next;
	while (ptr != NULL)
	{
		temp = ptr;
		del(temp->content);
		ptr = temp->next;
		free(temp);
	}
	free(*lst);
	*lst = NULL;
}

// void ft_lstcleartest()
// {
//     t_list *list = NULL;
//     t_list *node1 = malloc(sizeof(t_list));
//     t_list *node2 = malloc(sizeof(t_list));
//     t_list *node3 = malloc(sizeof(t_list));
//     t_list *node4 = malloc(sizeof(t_list));

//     char *content1 = strdup("one");
//     char *content2 = strdup("two");
//     char *content3 = strdup("three");
//     char *content4 = strdup("four");

//     node1->content = content1;
//     node1->next = node2;
//     node2->content = content2;
//     node2->next = node3;
//     node3->content = content3;
//     node3->next = node4;
//     node4->content = content4;
//     node4->next = NULL;

//     list = node1;

//     ft_lstclear(&list, free);
// 	//ft_lstclear(&node3, free);

// 	t_list *current = list;
// 	while (current != NULL) {
//     	printf("%s\n", (char *)current->content);
//     	current = current->next;
// 	}
// 	// casuística el nodo liberado es el primero
// 	//assert(list == NULL);
// 	printf("ft_lstclear: OK\n");
// }

// int main()
// {
//     ft_lstcleartest();
//     return 0;
// }
