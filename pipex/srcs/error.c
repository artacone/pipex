#include "../includes/pipex.h"

void	handle_error(int error_code)
{
	perror(NULL);
	exit(error_code);
}

void	handle_error_exec(char *cmd_name)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putendl_fd(cmd_name, 2);
	}
	else
	{
		perror(cmd_name);
	}
	exit(ERROR_EXEC);
}
