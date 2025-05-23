/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:15:37 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/23 19:24:46 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/*
Main forkes two process (since we expect to execute execve twice)
and calls child_cmd1 and child_cmd2.
Waits for them to finish. 
*/

// void	fork_failed(int p[2])
// {
// 	perror("Fork failed");
// 	close(p[0]);
// 	close(p[1]);
// 	exit(EXIT_FAILURE);
// }

// void exhaust_processes()
// {
//     while (1)
//     {
//         if (fork() == 0)
//         {
//             sleep(10);
//             exit(0);
//         }
//     }
// }

int	main(int argc, char **argv, char *const envp[])
{
	int		p[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status_cmd2;

	if (argc != 5)
		error_fd2("usage", "./pipex file1 cmd1 cmd2 file2",
			EXIT_FAILURE);
	if (pipe(p) == -1)
		exit (-1);
	pid1 = xfork();
	if (pid1 == 0)
		child1_cmd1(argv[1], p, argv[2], envp);
	pid2 = xfork();
	if (pid2 == 0)
		child2_cmd2(argv[4], p, argv[3], envp);
	xclose(p[0]);
	xclose(p[1]);
	waitpid(pid2, &status_cmd2, 0);
	waitpid(pid1, NULL, 0);
	if (WIFEXITED(status_cmd2))
		exit(WEXITSTATUS(status_cmd2));
	else
		exit(1);
}
