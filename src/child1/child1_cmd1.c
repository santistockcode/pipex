/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child1_cmd1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:48:50 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/23 19:21:25 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"

static void	open_infile1(char *filename, int *file1)
{
	*file1 = open(filename, O_RDONLY);
	if (*file1 == -1)
		fatal_sys(filename, 1);
}

// FIXME: differenciate 126 and 127
void	callexecve1(char *argv, char *const envp[])
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

void	child1_cmd1(char *file1, int p[2], char *argv, char *const envp[])
{
	int	fd1;

	open_infile1(file1, &fd1);
	xclose(p[0]);
	if (dup2(fd1, 0) == -1)
		fatal_sys("dup2 failed", 1);
	xclose(fd1);
	if (dup2(p[1], 1) == -1)
		fatal_sys("dup2 failed", 1);
	xclose(p[1]);
	callexecve1(argv, envp);
}
