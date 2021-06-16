#include "../includes/pipex.h"

void	ft_pipe(char **cmd1, char **cmd2, int fd_in, int fd_out)
{
	int		pipe_fds[2];
	pid_t	pid_1;
	pid_t	pid_2;

	if (pipe(pipe_fds) == -1)
	{
		exit(1); // pipe error
	}
	pid_1 = fork();
	if (pid_1 == -1)
	{
		exit(2); // fork error
	}
	if (pid_1 == 0) // Child process 1 (cmd1)
	{
		close(pipe_fds[0]);
		dup2(fd_in, 0);
		close(fd_in);

		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);

		execvp(cmd1[0], cmd1);
	}
	pid_2 = fork();
	if (pid_2 == -1)
	{
		exit(2); // fork error
	}
	if (pid_2 == 0) // Child process 2 (cmd2)
	{
		close(pipe_fds[1]);
		dup2(fd_out, 1);
		close(fd_out);

		dup2(pipe_fds[0], STDIN_FILENO);
		close(pipe_fds[0]);

		execvp(cmd2[0], cmd2);
	}
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	waitpid(pid_1, NULL, 0);
	waitpid(pid_2, NULL, 0);
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
		close(fd_in);
		close(fd_out);
	}
	return (0);
}