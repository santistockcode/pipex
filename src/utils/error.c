/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:12:45 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/20 18:35:16 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/utils.h"
#include "../../libft/include/libft.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../include/pipex.h"

void	error_stderror(char *context, char *description, int exit_status)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(context, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(description, 2);
	ft_putstr_fd("\n", 2);
	exit(exit_status);
}

void xfatal(const char *ctx, int exit_code)
{
    perror(ctx);
    exit(exit_code);
}

