#include "../includes/pipex.h"

void	ft_pipe(char **cmd1, char **cmd2, int fd_in, int fd_out)
{
	int		pipe_fds[2];
	pid_t	child_pid;

	if ((pipe(pipe_fds) == 0))
	{
		child_pid = fork();

		if (child_pid == 0) // Child
		{
			close(pipe_fds[1]);
			dup2(fd_out, 1);
			close(fd_out);

			dup2(pipe_fds[0], 0);
			close(pipe_fds[0]);

			execvp(cmd2[0], cmd2);
			close(0);
		}
		else if (child_pid == -1) // Error
		{
			write(2, "KEK\n", 4);
			// TODO
		}
		else // Parent
		{
			close(pipe_fds[0]);
			dup2(fd_in, 0);
			close(fd_in);

			dup2(pipe_fds[1], 1);
			close(pipe_fds[1]);

			execvp(cmd1[0], cmd1);
		}
	}
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
		fd_out = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT);
		ft_pipe(arg1, arg2, fd_in, fd_out);
	}
	return (0);
}