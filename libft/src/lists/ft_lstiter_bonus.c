/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:28:04 by saalarco          #+#    #+#             */
/*   Updated: 2024/12/21 10:41:58 by saalarco         ###   ########.fr       */
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

// typedef struct s_list
// {
// 	void			*content;
// 	struct s_list	*next;
// }	t_list;

// void	fortest_changeToLetterS(void *s)
// {
// 	char *str;
// 	str = (char *)s;
//     if (str == NULL)
//         return;  
//     for (int i = 0; str[i] != '\0'; i++)
//     {
//         str[i] = 's';
//     }
// }

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*ptr;

	if (!lst)
		return ;
	ptr = lst;
	while (ptr != NULL)
	{
		f(ptr->content);
		ptr = ptr->next;
	}
}

// void ft_lstitertest()
// {
//     t_list *list = NULL;
//     t_list *node1 = malloc(sizeof(t_list));
//     t_list *node2 = malloc(sizeof(t_list));
//     t_list *node3 = malloc(sizeof(t_list));
//     t_list *node4 = malloc(sizeof(t_list));

//     char *content1 = strdup("1");
//     char *content2 = strdup("10");
//     char *content3 = strdup("101");
//     char *content4 = strdup("1010");

//     node1->content = content1;
//     node1->next = node2;
//     node2->content = content2;
//     node2->next = node3;
//     node3->content = content3;
//     node3->next = node4;
//     node4->content = content4;
//     node4->next = NULL;

//     list = node1;
// 	t_list *current = list;
// 	while (current != NULL) {
//     	printf("%s\n", (char *)current->content);
//     	current = current->next;
// 	}
// 	ft_lstiter(list, fortest_changeToLetterS);
// 	current = list;
// 	while (current != NULL) {
//     	printf("%s\n", (char *)current->content);
//     	current = current->next;
// 	}
// 	printf("ft_lstiter: OK\n");
// }

// int main()
// {
//     ft_lstitertest();
//     return 0;
// }
