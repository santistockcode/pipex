/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child1_cmd1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:48:50 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/20 18:43:58 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"

void	callexecve1(char *argv, char *const envp[])
{
	char	**args;
	char	*path;

	args = ft_split(argv, ' ');
	if (access(args[0], 0) == 0)
		path = args[0];
	else
		path = path_from_cmdname(args[0], envp);
	if (path == NULL)
		error_stderror("child1_cmd1", "command not found", 127);
	if (execve(path, args, envp) == -1)
		perror("execve failed");
	exit(EXIT_FAILURE);
}

void	child1_cmd1(int file1, int p[2], char *argv, char *const envp[])
{
	close(p[0]);
	if (dup2(file1, 0) == -1)
	    xfatal("dup2 failed", 1);
	close(file1);
	if (dup2(p[1], 1) == -1)
	    xfatal("dup2 failed", 1);
	close(p[1]);
	callexecve1(argv, envp);
}
