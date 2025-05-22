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

static void open_infile1(char *filename, int *file1)
{
	*file1 = open(filename, O_RDONLY);
	if (*file1 == -1)
		fatal_sys(filename, 1);
}
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
		error_fd2(args[0], "command not found", 127);
	if (execve(path, args, envp) == -1)
		perror("execve failed"); // fixme: differenciate 126 and 127
	// free(path); is this correct?
	exit(EXIT_FAILURE);
}

void	child1_cmd1(char *file1, int p[2], char *argv, char *const envp[])
{
	int fd1;

	open_infile1(file1, &fd1);
	close(p[0]);
	if (dup2(fd1, 0) == -1)
	    fatal_sys("dup2 failed", 1);
	close(fd1);
	if (dup2(p[1], 1) == -1)
	    fatal_sys("dup2 failed", 1);
	close(p[1]);
	callexecve1(argv, envp);
}
