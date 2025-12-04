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

static void spawn_first_command(const char *infile, int p_read, int p_write, const char *cmd, char *const envp[])
{
    pid_t pid;
    int fd;

    pid = fork_wrap();
    if (pid < 0)
    {
        fatal_sys("fork", 1);
    }
    if (pid == 0)
    {
        fd = open_wrap(infile, O_RDONLY, 0);
        if (fd == -1)
        {
            perror(infile);
            exit(1);
        }
        if (dup2_wrap(fd, STDIN_FILENO) == -1)
            fatal_sys("dup2 infile", 1);
        safe_close(fd);
        if (dup2_wrap(p_write, STDOUT_FILENO) == -1)
            fatal_sys("dup2 p_write", 1);
        safe_close(p_read);
        safe_close(p_write);
        pipex_exec_cmd(cmd, envp);
    }
    safe_close(p_write);
}

static void spawn_mid_command(int in_fd, int p_read, int p_write, const char *cmd, char *const envp[])
{
    pid_t pid;

    pid = fork_wrap();
    if (pid < 0)
    {
        fatal_sys("fork", 1);
    }
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

    pid = fork_wrap();
    if (pid < 0)
    {
        fatal_sys("fork", 1);
    }
    if (pid == 0)
    {
        out_fd = open_wrap(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd == -1)
        {
            perror(outfile);
            exit(1);
        }
        if (dup2_wrap(in_fd, 0) == -1)
            fatal_sys("dup2 in_fd", 1);
        safe_close(in_fd);
        if (dup2_wrap(out_fd, 1) == -1)
            fatal_sys("dup2 out_fd", 1);
        safe_close(out_fd);
        pipex_exec_cmd(cmd, envp);
    }
    safe_close(in_fd);
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

    if (ctx->cmd_count == 0)
        return 1;
    
    /* First command: reads from infile */
    if (pipe_wrap(p) == -1)
        fatal_ctx("pipe", ctx, 1);
    PIPEX_LOG("pipe created p[0]=%d p[1]=%d for cmd[0]", p[0], p[1]);
    spawn_first_command(ctx->infile, p[0], p[1], ctx->commands[0], ctx->envp);
    PIPEX_LOG("spawned first command: %s", ctx->commands[0]);
    in_fd = p[0];
    
    /* Middle commands */
    i = 1;
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
    
    /* Last command: writes to outfile */
    status = spawn_last_command(in_fd, ctx->outfile, ctx->commands[ctx->cmd_count - 1], ctx->envp);
    PIPEX_LOG("last command: %s -> outfile=%s, status=%d", ctx->commands[ctx->cmd_count - 1], ctx->outfile, status);
    return status;
}
