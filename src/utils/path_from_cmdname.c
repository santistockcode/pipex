# include "../../include/utils.h"
# include "../../libft/include/libft.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
# include "../../include/pipex.h"


// recibe como arg "grep" por ejemplo
// this function looks for the envp that contains PATH= (bonus)
// and splits it by ':', then to join each path with the cmd (for example grep)
// if (access(path, X_OK) == 0) means existe
// entonces devuelvo el path (por ejemplo /usr/bin/grep)

char *path_from_cmdname(char *arg, char *const envp[])
{
    char **paths;
    char    *path;
    char    *bar;

    while(envp && *envp)
    {
        if (ft_strncmp(*envp, "PATH=", 5) == 0)
            break;
        envp++;
    }
    if (!(*envp))
        return(NULL);
    paths = ft_split(*envp + 5, ':');
    if (access(arg, 0) == 0)
        return (arg);
    while(paths && *paths)
    {
        bar = ft_strjoin(*paths, "/");
        path = ft_strjoin(bar, arg);
        free(bar);
        if (access(path, F_OK) == 0)
            return (path);
        free(path);
        paths++;
    }
    return (NULL);
}

void error(void)
{
    ft_printf("error manual");
    exit(EXIT_FAILURE);
}
