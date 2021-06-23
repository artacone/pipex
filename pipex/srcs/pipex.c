#include "../includes/pipex.h"

//void	ft_pipe(char **cmd1, char **cmd2, int fd_in, int fd_out)
//{
//	int		pipe_fds[2];
//	pid_t	pid_1;
//	pid_t	pid_2;
//
//	if (pipe(pipe_fds) == -1)
//	{
//		exit(1); // pipe error
//	}
//	pid_1 = fork();
//	if (pid_1 == -1)
//	{
//		exit(2); // fork error
//	}
//	if (pid_1 == 0) // Child process 1 (cmd1)
//	{
//		close(pipe_fds[0]);
//		dup2(fd_in, 0);
//		close(fd_in);
//
//		dup2(pipe_fds[1], STDOUT_FILENO);
//		close(pipe_fds[1]);
//
//		execvp(cmd1[0], cmd1);
//	}
//	pid_2 = fork();
//	if (pid_2 == -1)
//	{
//		exit(2); // fork error
//	}
//	if (pid_2 == 0) // Child process 2 (cmd2)
//	{
//		close(pipe_fds[1]);
//		dup2(fd_out, 1);
//		close(fd_out);
//
//		dup2(pipe_fds[0], STDIN_FILENO);
//		close(pipe_fds[0]);
//
//		execvp(cmd2[0], cmd2);
//	}
//	close(pipe_fds[0]);
//	close(pipe_fds[1]);
//	waitpid(pid_1, NULL, 0);
//	waitpid(pid_2, NULL, 0);
//}

int	main(int argc, char *argv[])
{
	int		fd_in;
	int		fd_out;
	char	**cmd;

	if (argc < 5)
	{
		return (1); // FIXME usage error
	}

	int		number_cmds = argc - 3;
	int		number_pipes = number_cmds - 1;
	pid_t	pids[number_cmds]; // FIXME malloc
	int		pipes[number_pipes][2]; // FIXME malloc

	// Create pipes
	for (int i = 0; i < number_pipes; i++)
	{
		if (pipe(pipes[i]) == -1) { return (2); } // pipe() error
	}

	// Create processes
	for (int i = 0; i < number_cmds; i++)
	{
		pids[i] = fork();
		if (pids[i] == -1) { return (3); } // fork() error

		if (pids[i] == 0) // i-th child
		{
			if (i == 0)
			{
				fd_in = open(argv[1], O_RDONLY);
				dup2(fd_in, STDIN_FILENO);
				close(fd_in);
				dup2(pipes[i][1], STDOUT_FILENO);
			}
			else if (i == number_cmds - 1)
			{
				fd_out = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777);
				dup2(fd_out, STDOUT_FILENO);
				close(fd_out);
				dup2(pipes[i - 1][0], STDIN_FILENO);
			}
			else
			{
				dup2(pipes[i - 1][0], STDIN_FILENO);
				dup2(pipes[i][1], STDOUT_FILENO);
			}
			// Close all extra pipes ends
			for (int j = 0; j < number_pipes; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}


			cmd = ft_split(argv[i + 2], ' '); // FIXME index ??

			execvp(cmd[0], cmd);
		}
	}
	// Close all pipes in parent process
	for (int j = 0; j < number_pipes; j++)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
	}
	for (int i = 0; i < number_cmds; i++)
	{
		waitpid(pids[i], NULL, 0);
	}

	return (0);
}