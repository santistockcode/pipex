/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child2_cmd2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:46:36 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/12 20:12:15 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/pipex.h"

void    callexecve2()
{
 char *const args[] = { "wc", "-l", NULL };
 char *const envp[] = { NULL };
 
 execve("/usr/bin/wc", args, envp);
 perror("execve");
    exit(EXIT_FAILURE);
}

void child2_cmd2(int file2, int p[2])
{
    close(p[1]);
    dup2(file2, 1);
    close(file2);
    dup2(p[0], 0);
    close(p[0]);
    callexecve2();
}