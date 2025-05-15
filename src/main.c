/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:15:37 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/15 12:15:14 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/pipex.h"

/*
Main forkes two process (since we expect to execute execve twice)
and calls child_cmd1 and child_cmd2.
Waits for them to finish. 
*/

int main(int argc, char **argv)
{
	int	p[2];
	pid_t	pid1;
	pid_t	pid2;
	int	file1;
	int file2;

	if (argc != 5)
		return(ft_printf("Wrong number of arguments\n"), 0);
	file1 = open(argv[1], O_RDONLY);
	if (file1 == -1)
		return(ft_printf("Error opening file %s\n", argv[1]), 0);
	file2 = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (file2 == -1)
		return(ft_printf("Error opening file %s\n", argv[argc - 1]), 0);
	if(pipe(p) == -1)
		exit(-1);
	// TODO: protect forks
	pid1 = fork();
	if(pid1 == 0)
		child1_cmd1(file1, p);
	pid2 = fork();
	if(pid2 == 0)
		child2_cmd2(file2, p);
	close(p[0]);
	close(p[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);	
}