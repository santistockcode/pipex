/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:15:37 by saalarco          #+#    #+#             */
/*   Updated: 2025/05/26 16:59:02 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"


static int usage_and_exit(void)
{
	error_fd2("usage", "./pipex infile cmd1 [cmd2 ... cmdN] outfile", EXIT_FAILURE);
	return (EXIT_FAILURE);
}

int	main(int argc, char **argv, char *const envp[])
{
	t_pipex_ctx ctx;
	int         status;

	if (argc < 5)
		return usage_and_exit();
	ctx = pipex_ctx_init(argc, argv, envp);
	status = pipex_run_pipeline(&ctx);
	pipex_ctx_free(&ctx);
	return status;
}
