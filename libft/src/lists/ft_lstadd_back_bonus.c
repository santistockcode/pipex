/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 19:11:19 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/06 16:30:20 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lists.h"

// #include <assert.h>
// #include <stdio.h>
// #include <stdlib.h>
// ft_lstnew
// ft_lstadd_front
// ft_lstlast

// typedef struct s_list
// {
// 	void			*content;
// 	struct s_list	*next;
// }	t_list;

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	if (!new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = ft_lstlast(*lst);
		temp->next = new;
	}
}

/*
Aunque podamos pensar que hay que terminarlo en nulo
como sucede con los strings, los nodos son una capa
de abstracción, esto es, el nodo que nos pasan para 
añadir al final puede estár COMPUESTO de 8 nodos
y si hago un new->next = NULL entonces me cargo 4
nodos finales y la lista se corrompe
*/

// int main()
// {
// 	t_list *list = NULL; 
//     t_list *node1 = ft_lstnew("UNO");
//     t_list *node2 = ft_lstnew("DOS");

//     t_list  *listfinal;

//     ft_lstadd_front(&list, node1);
//     ft_lstadd_front(&list, node2);

//     ft_lstadd_back(&listfinal, list);
//     t_list *current = listfinal;
//     int counter = 0;

//     while (current != NULL)
//     {
//         printf("%s\n", (char *)current->content);
//         t_list *temp = current->next;
//         current = temp;
//         counter++;
//     }

//     // Free the memory
//     current = listfinal;
//     while (current != NULL)
//     {
//         t_list *temp = current->next;
//         free(current);
//         current = temp;
//     }

//     printf("Tests ok\n");

// return 0;
// }