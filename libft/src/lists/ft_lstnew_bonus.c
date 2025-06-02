/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:21:33 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/06 16:32:06 by saalarco         ###   ########.fr       */
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

t_list	*ft_lstnew(void *content)
{
	t_list		*response_node;

	response_node = (t_list *) malloc (sizeof(t_list));
	if (response_node == NULL)
		return (NULL);
	response_node->content = content;
	response_node->next = NULL;
	return (response_node);
}

/*
Si no existe asigna content a NULL,
en este punto no es importante pero sí en funciones
que siguen
*/

// int	main()
// {
// 	t_list *lista_terminada_en_null;
// 	lista_terminada_en_null = ft_lstnew("here is my list");
// 	assert(strcmp(lista_terminada_en_null->content,"here is my list") == 0);
// 	assert(lista_terminada_en_null->next == NULL);
// 	printf("Tests ok");
// }
