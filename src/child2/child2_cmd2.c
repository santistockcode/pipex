/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child2_cmd2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:46:36 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/23 19:23:24 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"

static void	open_infile2(char *filename, int *file2)
{
	*file2 = open(filename, O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (*file2 == -1)
		fatal_sys(filename, 1);
}

void	callexecve2(char *argv, char *const envp[])
{
	char	**args;
	char	*path;

	if (!argv || argv[0] == '\0')
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd("Command '' not found\n", 2);
		exit(127);
	}
	args = ft_split(argv, ' ');
	if (access(args[0], 0) == 0)
		path = ft_strdup(args[0]);
	else
		path = path_from_cmdname(args[0], envp);
	if (path == NULL)
	{
		custom_error_fd2(args[0], args, 127);
	}
	if (execve(path, args, envp) == -1)
		perror("execve failed");
	free(path);
	ft_split_free(args);
	exit(EXIT_FAILURE);
}

void	child2_cmd2(char *file2, int p[2], char *argv, char *const envp[])
{
	int	fd2;

	open_infile2(file2, &fd2);
	xclose(p[1]);
	if (dup2(fd2, 1) == -1)
		fatal_sys("dup2 failed", 1);
	xclose(fd2);
	if (dup2(p[0], 0) == -1)
		fatal_sys("dup2 failed", 1);
	xclose(p[0]);
	callexecve2(argv, envp);
}
