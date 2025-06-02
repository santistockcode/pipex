/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:23:31 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/23 19:29:00 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <unistd.h>

// where cmd is the name of the program"ls"
// Caller is responsible for freeing the returned string
char	*path_from_cmdname(char *cmd, char *const envp[]);
void	ft_split_free(char **paths);

void	error_fd2(char *context, char *description, int exit_status);
void	custom_error_fd2(char *context, char **args, int exit_status);

// wrapper using strerror
void	fatal_sys(const char *context, int exit_code);
// wrapper for fork and close in case syscalls fails (raro)
pid_t	xfork(void);
void	xclose(int fd);

#endif