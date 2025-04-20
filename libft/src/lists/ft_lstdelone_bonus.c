/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:26:25 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/06 16:31:50 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lists.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	del(lst->content);
	free(lst);
}
/*
lst es el nodo a liberar
del es un puntero a la función que se utiliza para liberar el nodo
*/

// int main()
// {
// 	char *str1 = ft_strdup("uno");
// 	char *str2 = ft_strdup("dos");
// 	char *str3 = ft_strdup("tres");
// 	char *str4 = ft_strdup("cuatro");

// 	t_list *node1 = ft_lstnew(str1);
// 	t_list	*node2 = ft_lstnew(str2);
// 	t_list	*node3 = ft_lstnew(str3);
// 	t_list	*node4 = ft_lstnew(str4);

// 	node1->next = node2;
// 	node2->next = node3;
// 	node3->next = node4;

// t_list	*current = node1;
// int 	counter;
// counter = 0;
// while (current != NULL)
// {
// 	printf("CONTENT: %s\n", (char *)current->content);
// 	t_list *temp = current->next;
// 	current = temp;
// 	counter++;
// }
// ft_lstdelone(node2, &ft_del);
// printf("\n%s\n", "Si llamamos a ft_lstdelone: segmentation fault.");
// printf("\nAUNQUE node3 sigue siendo: %s\n", (char *) node3->content);

// current = node1;
// while (current != NULL)
// {
// 	printf("CONTENT: %s\n", (char *)current->content);
// 	t_list *temp = current->next;
// 	current = temp;
// 	counter++;
// }

// // current = elem;
// // while (current != NULL)
// // {
// // t_list *temp = current->next;
// // free(current);
// // current = temp;
// // }
// return (0);
// }
