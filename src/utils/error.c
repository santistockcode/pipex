/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:12:45 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/22 19:28:10 by saalarco         ###   ########.fr       */
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
	ft_putstr_fd("pipex: ", 2);
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

// void xfatal(const char *ctx, int exit_code)
// {
//     perror(ctx);
//     exit(exit_code);
// }
