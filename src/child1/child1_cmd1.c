/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child1_cmd1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:48:50 by saalarco          #+#    #+#             */
/*   Updated: 2025/04/20 14:05:39 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/pipex.h"

void    callexecve1()
{
 char *const args[] = { "ls", NULL };
 char *const envp[] = { NULL };
 ft_printf("call1");
 execve("/usr/bin/ls", args, envp);
 perror("execve");
    exit(EXIT_FAILURE);
}

void child1_cmd1(int file1, int p[2])
{
    close(p[0]);
    dup2(file1, 0);
    close(file1);
    dup2(p[1], 1);
    close(p[1]);
    callexecve1();
}