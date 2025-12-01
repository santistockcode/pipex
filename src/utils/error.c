/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:12:45 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/26 17:00:16 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/utils.h"
#include "../../libft/include/libft.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../include/pipex.h"

// Prints "<context>: command not found" to stderr, frees args, and exits.
// Use for user-level errors like an unknown command string.
void	pipex_exit_cmd_not_found(char *context, char **args, int exit_status)
{
	ft_putstr_fd(context, 2);
	ft_putstr_fd(": command not found\n", 2);
	ft_split_free(args);
	exit(exit_status);
}

// Prints a branded error: "pipex: <context>: <description>" to stderr and exits.
// Suitable for reporting errors where you provide a human description.
void	error_fd2(char *context, char *description, int exit_status)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(context, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(description, 2);
	ft_putstr_fd("\n", 2);
	exit(exit_status);
}

// Syscall failure helper: uses strerror(errno) with your context and exits.
// Example: if pipe() fails, call fatal_sys("pipe", 1).
void	fatal_sys(const char *context, int exit_code)
{
	error_fd2((char *) context, strerror(errno), exit_code);
}

// Non-fatal close: useful during cleanup paths where some fds
// may already be closed or invalid. Avoids crashing on -1.
// Logs failures with perror but continues execution.
void	safe_close(int fd)
{
	if (fd >= 0)
	{
		if (close(fd) == -1)
		{
			// Report, but do not exit; keep cleanup progressing
			perror("close failed");
		}
	}
}

// Strict fork wrapper: if fork() fails, report via fatal_sys and exit.
// Returns child's PID in the parent, 0 in the child.
pid_t	xfork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		fatal_sys("fork", 1);
	return (pid);
}

// Centralized execve failure handling: prints via perror,
// decides proper exit code (127: not found, 126: not executable/other),
// frees transient allocations and exits. Intended for CHILD process only.
void	pipex_handle_execve_error(char **args, char *resolved_path,
								const char *context)
{
	(void)context; // context string optional; perror uses errno
	perror("execve failed");
	// Bash conventions: 127 when command not found (ENOENT), else 126
	int code = (errno == ENOENT) ? 127 : 126;
	if (resolved_path)
		free(resolved_path);
	if (args)
		ft_split_free(args);
	exit(code);
}

// Parent-side fatal helper: frees application context, then prints
// error (using strerror) and exits with the provided code.
// Use this when a fatal error occurs in the parent and you must
// release shared resources (heap allocations inside t_pipex_ctx).
void	fatal_ctx(const char *context, t_pipex_ctx *ctx, int exit_code)
{
	if (ctx)
		pipex_ctx_free(ctx);
	fatal_sys(context, exit_code);
}
