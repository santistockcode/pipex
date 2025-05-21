/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child2_cmd2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:46:36 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/20 18:36:19 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/pipex.h"

static void open_infile2(char *filename, int *file2)
{
	*file2 = open(filename, O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (*file2 == -1)
	{
        fprintf(stderr, "pipex: %s: %s\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void    callexecve2(char *argv, char *const envp[])
{
    char **args;
    char *path;

    args = ft_split(argv, ' ');
    if (access(args[0], 0) == 0)
		path = args[0];
    else
        path = path_from_cmdname(args[0], envp);
    if (path == NULL)
        error_stderror("child2_cmd2", "command not found", 127);
    if (execve(path, args, envp) == -1)
        perror("execve failed");
    // how to free path here?
    exit(EXIT_FAILURE);
}

void child2_cmd2(char *file2, int p[2], char *argv, char *const envp[])
{
    int fd2;

    open_infile2(file2, &fd2);
    close(p[1]);
    if (dup2(fd2, 1) == -1)
        xfatal("dup2 failed", 1);
    close(fd2);
    if (dup2(p[0], 0) == -1)
	    xfatal("dup2 failed", 1);
    close(p[0]);
    callexecve2(argv, envp);
}