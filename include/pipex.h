/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:20:52 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/06 18:42:36 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

// libft
# include "../libft/include/libft.h"

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


void child1_cmd1(int file1, int p[2]);
void child2_cmd2(int file1, int p[2]);


#endif