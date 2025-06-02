/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:52:01 by saalarco          #+#    #+#             */
/*   Updated: 2024/02/02 11:52:04 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// #include <errno.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <unistd.h>

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

// int main () {
//     int fd = open("foo.txt", O_WRONLY | O_CREAT);
//     ft_putchar_fd('a', fd);
// 	printf("%d", fd);
//     if (fd == -1) {
//         printf("Error Number % d\n", errno);
//     }
//    return(0);
// }