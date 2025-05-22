#ifndef UTILS_H
# define UTILS_H

// where cmd is the name of the program"ls"
// Caller is responsible for freeing the returned string
char *path_from_cmdname(char *cmd, char *const envp[]);


void	error_fd2(char *context, char *description, int exit_status);

// // wrapper for perror
// void xfatal(const char *ctx, int exit_code);

// wrapper using strerror
void fatal_sys(const char *context, int exit_code);


#endif