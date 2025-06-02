/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 16:48:24 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 10:42:11 by saalarco         ###   ########.fr       */
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
// ft_lstdelone
// ft_lstadd_back

// typedef struct s_list
// {
// 	void			*content;
// 	struct s_list	*next;
// }	t_list;

// void	*take_str_give_me_e(void *s)
// {
// 	char *str;

// 	str = (char *)s;
//     if (str == NULL)
//         return NULL;  
//     for (int i = 0; str[i] != '\0'; i++)
//     {
//         str[i] = 's';
//     }
// 	return (str);
// }
// void delete(void *content)
// {
// 	free(content);
// }

/*
Si guardo result_f es por si al crearla
falla la alocacion de memoria, según los tests
debiere igualmente aplicar f previo a la reserva
*/

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *),
		void (*del)(void *))
{
	t_list	*ptr;
	t_list	*ptr_new;
	t_list	*res_lst;
	void	*result_f;

	if (!lst || !del || !f)
		return (NULL);
	ptr = lst;
	res_lst = NULL;
	while (ptr != NULL)
	{
		result_f = f(ptr->content);
		ptr_new = ft_lstnew(result_f);
		if (ptr_new == NULL)
		{
			del(result_f);
			ft_lstclear(&res_lst, (*del));
			return (NULL);
		}
		ft_lstadd_back (&res_lst, ptr_new);
		ptr = ptr->next;
	}
	return (res_lst);
}

// int main()
// {
//     t_list *list = NULL;
//     t_list *temp;

//     // Crea la linked list
//     list = (t_list *)malloc(sizeof(t_list));
//     list->content = strdup("one");
// 		list->next = NULL;
//     // list->next = (t_list *)malloc(sizeof(t_list));
//     // temp = list->next;
//     // temp->content = strdup("two");
//     // temp->next = (t_list *)malloc(sizeof(t_list));
//     // temp = temp->next;
//     // temp->content = strdup("three");
//     // temp->next = (t_list *)malloc(sizeof(t_list));
//     // temp = temp->next;
//     // temp->content = strdup("four");
//     //temp->next = NULL;

// 	temp = list;
// 	 while (temp != NULL)
//     {
//         printf("%s\n", (char *)temp->content);
//         temp = temp->next;
//     }

//     // Test ft_strmapi function
//     t_list *result = ft_lstmap(list, take_str_give_me_e, delete);

//     // Print the result
//     temp = result;
//     while (temp != NULL)
//     {
//         printf("%s\n", (char *)temp->content);
//         temp = temp->next;
//     }

//     // Free the memory
//     while (list != NULL)
//     {
//         temp = list;
//         list = list->next;
//         delete(temp->content);
//         free(temp);
//     }
//     return 0;
// }
