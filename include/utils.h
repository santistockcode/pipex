#ifndef UTILS_H
# define UTILS_H

// where cmd is the name of the program"ls"
// Caller is responsible for freeing the returned string
char *path_from_cmdname(char *cmd, char *const envp[]);

#endif