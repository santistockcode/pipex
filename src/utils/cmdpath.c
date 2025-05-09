# include "../../include/utils.h"
# include "../../libft/include/libft.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// made by copilot 
char *cmd_resolve_path(const char *cmd, char *const envp[])
{
    char	*path;
    char	*full_path;
    char	*path_env;
    char	**paths;
    int		i;

    path_env = NULL;
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            path_env = envp[i] + 5;
        i++;
    }
    if (!path_env)
        return (NULL);
    paths = ft_split(path_env, ':');
    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin(paths[i], "/");
        path = ft_strjoin(full_path, cmd);
        free(full_path);
        if (access(path, X_OK) == 0)
            break ;
        free(path);
        i++;
    }
    free(paths);
    if (!paths[i])
        return (NULL);
    return (path);
}