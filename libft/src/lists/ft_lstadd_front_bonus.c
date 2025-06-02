/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:50:03 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/06 16:31:42 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lists.h"

// #include <assert.h>
// #include <stdio.h>
// #include <stdlib.h>
// ft_lstnew
// ft_lstadd_front
// ft_lstlast

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst)
		new->next = NULL;
	else
	{
		new->next = *lst;
	}
	*lst = new;
}

// int main()
// {
// 	t_list *list = NULL; 
//     t_list *node1 = ft_lstnew("Estaba primero");
//     t_list *node2 = ft_lstnew("Entra después");

//     ft_lstadd_front(&list, node1);
//     ft_lstadd_front(&list, node2);

//     t_list *current = list;
//     int counter = 0;

//     while (current != NULL)
//     {
//         switch (counter)
//         {
//         case 0:
// 			assert(strcmp(current->content, "Entra después") == 0);
//             //printf("%s\n", (char *)current->content);
//             break;
//         case 1:
// 			assert(strcmp(current->content, "Estaba primero") == 0);
//             // printf("%s\n", (char *)current->content);
//             break;
//         }

//         t_list *temp = current->next;
//         current = temp;
//         counter++;
//     }

//     // Free the memory
//     current = list;
//     while (current != NULL)
//     {
//         t_list *temp = current->next;
//         free(current);
//         current = temp;
//     }

//     printf("Tests ok\n");

// return 0;
// }
