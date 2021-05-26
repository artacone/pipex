#include "../includes/pipex.h"

void	ft_pipe(char **cmd1, char **cmd2, int fd_in, int fd_out)
{
	int	fds[2];
	pid_t	child_pid;
	if (pipe)
}

int	main(int argc, char *argv[])
{
	int		fd_in;
	int		fd_out;
	char	**arg1;
	char	**arg2;

	if (argc == 5)
	{
		arg1 = ft_split(argv[2], ' ');
		arg2 = ft_split(argv[3], ' ');
		fd_in = open(argv[1], O_RDONLY);
		fd_out = open(argv[4], O_WRONLY | O_TRUNC);
		ft_pipe(arg1, arg2, fd_in, fd_out);
	}
	return (0);
}