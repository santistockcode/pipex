/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:15:37 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/20 18:41:34 by saalarco         ###   ########.fr       */
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

void	init_files(int argc, char **argv, int *file1, int *file2)
{
	if (argc != 5)
		error_stderror("main", "Usage: ./pipex file1 cmd1 cmd2 file2",
			EXIT_FAILURE);
	*file1 = open(argv[1], O_RDONLY);
	if (*file1 == -1)
		error_stderror("main", "Error opening file1", 1);
	*file2 = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (*file2 == -1)
		error_stderror("main", "Error opening file2", 1);
}

int	main(int argc, char **argv, char *const envp[])
{
	int		p[2];
	pid_t	pid1;
	pid_t	pid2;
	int		file1;
	int		file2;
	int		statusCmd2;

	init_files(argc, argv, &file1, &file2);
	if (pipe(p) == -1)
		exit (-1);
	pid1 = fork();
	if (pid1 == 0)
		child1_cmd1(file1, p, argv[2], envp);
	pid2 = fork();
	if (pid2 == 0)
		child2_cmd2(file2, p, argv[3], envp);
	close(file1);
	close(file2);
	close(p[0]);
	close(p[1]);
	waitpid(pid2, &statusCmd2, 0);
	waitpid(pid1, NULL, 0);
	if (WIFEXITED(statusCmd2))
		exit(WEXITSTATUS(statusCmd2));
	else
		exit(1);
}
