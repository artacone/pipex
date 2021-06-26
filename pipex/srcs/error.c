#include "../includes/pipex.h"

void	handle_error(int error_code, char *str)
{
	ft_putstr_fd("pipex: ", 2);
	if (error_code == ERROR_EXEC && errno == ENOENT)
	{
		ft_putstr_fd(str, 2);
		ft_putendl_fd(": Command not found", 2);
	}
	else
	{
		perror(str);
	}
	exit(error_code);
}
