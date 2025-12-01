/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 12:20:00 by saalarco          #+#    #+#             */
/*   Updated: 2025/11/30 12:20:00 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"
#include "../../libft/include/libft.h"

static char **dup_commands(int count, char **argv_base)
{
    char **out;
    int i;

    out = (char **)malloc(sizeof(char *) * count);
    if (!out)
        fatal_sys("malloc", 1);
    i = 0;
    while (i < count)
    {
        out[i] = ft_strdup(argv_base[i]);
        if (!out[i])
            fatal_sys("ft_strdup", 1);
        i++;
    }
    return out;
}

static void free_commands(int count, char **cmds)
{
    int i;

    if (!cmds)
        return;
    i = 0;
    while (i < count)
    {
        if (cmds[i])
            free(cmds[i]);
        i++;
    }
    free(cmds);
}

t_pipex_ctx pipex_ctx_init(int argc, char **argv, char *const envp[])
{
    t_pipex_ctx ctx;
    int cmdn;

    cmdn = argc - 3;
    ctx.infile = argv[1];
    ctx.outfile = argv[argc - 1];
    ctx.cmd_count = cmdn;
    ctx.envp = envp;
    ctx.commands = dup_commands(cmdn, &argv[2]);
    return ctx;
}

void pipex_ctx_free(t_pipex_ctx *ctx)
{
    if (!ctx)
        return;
    free_commands(ctx->cmd_count, ctx->commands);
}
