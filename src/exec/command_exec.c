/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 12:22:00 by saalarco          #+#    #+#             */
/*   Updated: 2025/11/30 12:22:00 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"
#include <errno.h>

char **pipex_split_args(const char *cmd)
{
    return ft_split(cmd, ' ');
}

char *pipex_resolve_path(char **args, char *const envp[])
{
    char *path;

    if (!args || !args[0] || args[0][0] == '\0')
        return NULL;
    if (access(args[0], 0) == 0)
        path = ft_strdup(args[0]);
    else
        path = path_from_cmdname(args[0], envp);
    return path;
}

void pipex_exec_cmd(const char *cmd, char *const envp[])
{
    char **args;
    char *path;

    args = pipex_split_args(cmd);
    if (!args || !args[0] || args[0][0] == '\0')
    {
        ft_putstr_fd("pipex: ", 2);
        ft_putstr_fd("Command '' not found\n", 2);
        ft_split_free(args);
        exit(127);
    }
    path = pipex_resolve_path(args, envp);
    if (path == NULL)
        pipex_exit_cmd_not_found(args[0], args, 127);
    if (execve(path, args, envp) == -1)
        pipex_handle_execve_error(args, path, "execve");
    // Not reached if execve succeeds
    free(path);
    ft_split_free(args);
    exit(EXIT_FAILURE);
}
