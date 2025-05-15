# include "../../include/utils.h"
# include "../../libft/include/libft.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// recibe como arg "grep" por ejemplo
// this function looks for the envp that contains PATH= (bonus)
// and splits it by ':', then to join each path with the cmd (for example grep)
// if (access(path, X_OK) == 0) means existe
// entonces devuelvo el path (por ejemplo /usr/bin/grep)

char *path_from_cmdname(char *arg, char *const envp[])
{
    char **paths;

    while(envp)
    {
        if (ft_strncmp(*envp, "PATH=", 5) == 0)
            break;
        envp++;
    }
    paths = ft_split(*envp + 5, ':');
    if (!paths)
        return (NULL);
    ft_printf("%s", arg);
    return (NULL);
}