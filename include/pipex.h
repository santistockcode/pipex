/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:20:52 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/22 19:24:06 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

// libft
# include "../libft/include/libft.h"

// utils
# include "utils.h"

// open unlink
# include <fcntl.h>

/*
	read
	write
	close
	access
	pipe
	fork
	execve
	dup
	dup2
*/
# include <unistd.h>

// wait waitpid
# include <sys/wait.h>

// malloc free exit
# include <stdlib.h>

// perror
# include <stdio.h>

// strerror
# include <string.h>
# include <errno.h>

typedef struct s_pipex_ctx
{
	char            *infile;
	char            *outfile;
	char           **commands;   // deep-copied array of command strings
	int              cmd_count;
	char *const     *envp;
}   t_pipex_ctx;

// Lifecycle
t_pipex_ctx pipex_ctx_init(int argc, char **argv, char *const envp[]);
void        pipex_ctx_free(t_pipex_ctx *ctx);

// Execution
int         pipex_run_pipeline(t_pipex_ctx *ctx);

// Command exec helpers
char      **pipex_split_args(const char *cmd);
char       *pipex_resolve_path(char **args, char *const envp[]);
void        pipex_exec_cmd(const char *cmd, char *const envp[]);

// Legacy child helpers (kept for compatibility if needed)
void	child1_cmd1(char *file1, int p[2], char *argv, char *const envp[]);
void	child2_cmd2(char *file2, int p[2], char *argv, char *const envp[]);


// Parent-side fatal: free context before exiting with error (lives here because of t_pipex_ctx)
void	fatal_ctx(const char *context, t_pipex_ctx *ctx, int exit_code);

#endif