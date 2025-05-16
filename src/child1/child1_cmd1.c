/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child1_cmd1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:48:50 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/16 18:38:03 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/pipex.h"

void    callexecve1(char *argv, char *const envp[])
{
    char **args;
    char *path;

    args = ft_split(argv, ' ');
    path = path_from_cmdname(args[0], envp);
    // TODO: Protect
    if (execve(path, args, envp) == -1)
        perror("execve failed");
    // habría que liberar path (importante)
    exit(EXIT_FAILURE);
}

void child1_cmd1(int file1, int p[2], char *argv, char *const envp[])
{
    close(p[0]);
    dup2(file1, 0);
    close(file1);
    dup2(p[1], 1);
    close(p[1]);
    callexecve1(argv, envp);
}