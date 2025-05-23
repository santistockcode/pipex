/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:12:45 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/23 19:30:03 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/utils.h"
#include "../../libft/include/libft.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../include/pipex.h"

void	custom_error_fd2(char *context, char **args, int exit_status)
{
	ft_putstr_fd(context, 2);
	ft_putstr_fd(": command not found\n", 2);
	ft_split_free(args);
	exit(exit_status);
}

void	error_fd2(char *context, char *description, int exit_status)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(context, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(description, 2);
	ft_putstr_fd("\n", 2);
	exit(exit_status);
}

void	fatal_sys(const char *context, int exit_code)
{
	error_fd2((char *) context, strerror(errno), exit_code);
}

void	xclose(int fd)
{
	if (close(fd) == -1)
		fatal_sys("close", 1);
}

pid_t	xfork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		fatal_sys("fork", 1);
	return (pid);
}
