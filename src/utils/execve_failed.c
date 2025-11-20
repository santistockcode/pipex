#include "../../include/utils.h"
#include "../../libft/include/libft.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../include/pipex.h"

void	execve_failed(const char *context)
{

	int code = (errno == ENOENT) ? 127 : 126; 

	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd((char *) context, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	exit(code);
}