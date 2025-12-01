/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_builder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 12:25:00 by saalarco          #+#    #+#             */
/*   Updated: 2025/11/30 12:25:00 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"
#include "../../include/log.h"
#include "../../include/syswrap.h"

static int open_infile_fd(const char *infile)
{
    int fd;

    fd = open(infile, O_RDONLY);
    if (fd == -1)
    {
        // Mimic bash: report error but continue pipeline; feed /dev/null
        perror(infile);
        fd = open("/dev/null", O_RDONLY);
        if (fd == -1)
            fatal_sys("/dev/null", 1);
    }
    return fd;
}

static int open_outfile_fd(const char *outfile)
{
    int fd;

    fd = open(outfile, O_TRUNC | O_CREAT | O_RDWR, 0000644);
    if (fd == -1)
        fatal_sys((char *)outfile, 1);
    return fd;
}

static void spawn_mid_command(int in_fd, int p_read, int p_write, const char *cmd, char *const envp[])
{
    pid_t pid;

    pid = fork_wrap();
    if (pid == 0)
    {
        if (dup2_wrap(in_fd, 0) == -1)
            fatal_sys("dup2 in_fd", 1);
        safe_close(in_fd);
        if (dup2_wrap(p_write, 1) == -1)
            fatal_sys("dup2 p_write", 1);
        safe_close(p_read);
        safe_close(p_write);
        pipex_exec_cmd(cmd, envp);
    }
    safe_close(in_fd);
    safe_close(p_write);
}

static int spawn_last_command(int in_fd, const char *outfile, const char *cmd, char *const envp[])
{
    pid_t pid;
    int out_fd;
    int status;

    out_fd = open_outfile_fd(outfile);
    pid = fork_wrap();
    if (pid == 0)
    {
        if (dup2_wrap(in_fd, 0) == -1)
            fatal_sys("dup2 in_fd", 1);
        safe_close(in_fd);
        if (dup2_wrap(out_fd, 1) == -1)
            fatal_sys("dup2 out_fd", 1);
        safe_close(out_fd);
        pipex_exec_cmd(cmd, envp);
    }
    safe_close(in_fd);
    safe_close(out_fd);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 1;
}

int pipex_run_pipeline(t_pipex_ctx *ctx)
{
    int in_fd;
    int p[2];
    int i;
    int status;

    in_fd = open_infile_fd(ctx->infile);
    PIPEX_LOG("infile fd=%d", in_fd);
    i = 0;
    while (i < ctx->cmd_count - 1)
    {
        if (pipe_wrap(p) == -1)
            fatal_ctx("pipe", ctx, 1);
        PIPEX_LOG("pipe created p[0]=%d p[1]=%d for cmd[%d]", p[0], p[1], i);
        spawn_mid_command(in_fd, p[0], p[1], ctx->commands[i], ctx->envp);
        PIPEX_LOG("spawned mid command idx=%d: %s", i, ctx->commands[i]);
        in_fd = p[0];
        i++;
    }
    status = spawn_last_command(in_fd, ctx->outfile, ctx->commands[ctx->cmd_count - 1], ctx->envp);
    PIPEX_LOG("last command: %s -> outfile=%s, status=%d", ctx->commands[ctx->cmd_count - 1], ctx->outfile, status);
    return status;
}
