#ifndef UTILS_H
# define UTILS_H

// where cmd is command like "grep", returns NULL if not found
// Caller is responsible for freeing the returned string
char *cmd_resolve_path(const char *cmd, char *const envp[]);

#endif