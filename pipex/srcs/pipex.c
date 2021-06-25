#include "../includes/pipex.h"

/*
 * TODO free stuff
 * bonus <<>>
 */

void	wait_for_children(t_manager *manager, int n)
{
	for (int i = 0; i < n; i++)
	{
		waitpid((manager->pids)[i], NULL, 0);
	}
}

void	handle_error(int error_code)
{
	perror(NULL);
	exit(error_code);
}

void	handle_error_exec(char *cmd_name)
{
	ft_putstr_fd("command not found: ", 2);
	ft_putendl_fd(cmd_name, 2);
	exit(ERROR_EXEC);
}

void	create_pipes(t_manager *manager)
{
	int	**pipes;

	pipes = (int **)malloc(manager->n_pipes * sizeof(int *));
	if (pipes == NULL)
	{
		handle_error(ERROR_ALLOC);
	}
	for (int i = 0; i < manager->n_pipes; i++)
	{
		pipes[i] = (int *)malloc(2 * sizeof(int));
		if (pipes[i] == NULL)
		{
			handle_error(ERROR_ALLOC);
		}
	}
	// Open pipes
	for (int i = 0; i < manager->n_pipes; i++)
	{
		if (pipe(pipes[i]) == -1)
		{
			handle_error(ERROR_PIPE);
		}
	}
	manager->pipes = pipes;
}

t_manager	*init_manager(int argc, char *argv[], char *envp[])
{
	t_manager	*manager;

	manager = (t_manager *)malloc(sizeof(*manager));
	if (manager == NULL)
	{
		handle_error(ERROR_ALLOC);
	}
	manager->argc = argc;
	manager->argv = argv;
	manager->envp = envp;
	manager->n_cmds = argc - 3;
	manager->n_pipes = manager->n_cmds - 1;
	manager->pids = (pid_t *)malloc(manager->n_cmds * sizeof(pid_t));
	if (manager->pids == NULL)
	{
		handle_error(ERROR_ALLOC);
	}
	create_pipes(manager);
	return (manager);
}

void	close_pipes(t_manager *manager)
{
	int	n_pipes;
	int	**pipes;

	n_pipes = manager->n_pipes;
	pipes = manager->pipes;
	for (int i = 0; i < n_pipes; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

char	*get_cmd_path(t_manager *manager, char **cmd)
{
	char	*cmd_path;
	char	**path;

	cmd_path = NULL;
	path = NULL;
	for (int i = 0; (manager->envp)[i] != NULL; i++)
	{
		if (ft_strncmp("PATH=", (manager->envp)[i], 5) == 0)
		{
			char *path_string = (manager->envp)[i];
			path = ft_split(ft_strchr(path_string, '=') + 1, ':');
			if (path == NULL)
			{
				handle_error(ERROR_ALLOC);
			}
			break ;
		}
	}

	if (path != NULL)
	{
		for (char *curr_path = *path; curr_path != NULL; curr_path = *(++path))
		{
			cmd_path = ft_strjoin(curr_path, "/");
			cmd_path = ft_strjoin(cmd_path, cmd[0]);
			if (access(cmd_path, X_OK) == 0)
			{
				return (cmd_path);
			}
		}
	}
	return (cmd_path);
}

void	execute_cmd(t_manager *manager, int i)
{
	char	**cmd;

	close_pipes(manager);

	cmd = ft_split((manager->argv)[i + 2], ' ');
	if (cmd == NULL)
	{
		handle_error(ERROR_ALLOC);
	}
	int	is_path = 0;
	if (ft_strncmp(cmd[0], "/", 1) == 0
		|| ft_strncmp(cmd[0], ".", 1) == 0)
	{
		is_path = 1;
	}
	if (is_path)
	{
		execve(cmd[0], cmd, manager->envp);
	}
	else
	{
		char	*cmd_path;
		cmd_path = get_cmd_path(manager, cmd);
		execve(cmd_path, cmd, manager->envp);
	}
	handle_error_exec(cmd[0]);
}

void	execute_cmd1(t_manager *manager)
{
	int	fd_in;
	int	**pipes;

	pipes = manager->pipes;
	fd_in = open((manager->argv)[1], O_RDONLY); // FIXME handle error
	if (fd_in == -1)
	{
		handle_error(ERROR_OPEN);
	}
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
	dup2(pipes[0][1], STDOUT_FILENO);
	execute_cmd(manager, 0);
}

void	execute_cmdn(t_manager *manager)
{
	int	fd_out;
	int	**pipes;

	fd_out = open((manager->argv)[manager->argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (fd_out == -1)
	{
		handle_error(ERROR_OPEN);
	}
	pipes = manager->pipes;
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	dup2(pipes[manager->n_pipes - 1][0], STDIN_FILENO);
	execute_cmd(manager, manager->n_cmds - 1);
}

void	ft_pipex(t_manager *manager)
{
	pid_t	*pids;
	int 	**pipes;

	pids = manager->pids;
	pipes = manager->pipes;
	for (int i = 0; i < manager->n_cmds; i++)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			wait_for_children(manager, i);
			handle_error(ERROR_FORK);
		}
		if (pids[i] == 0) // i-th child
		{
			if (i == 0)
				execute_cmd1(manager);
			else if (i == manager->n_cmds - 1)
				execute_cmdn(manager);
			else
			{
				dup2(pipes[i - 1][0], STDIN_FILENO);
				dup2(pipes[i][1], STDOUT_FILENO);
				execute_cmd(manager, i);
			}
		}
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_manager	*manager;

	if (argc < 5)
	{
		ft_putendl_fd("USAGE: ./pipex infile cmd1 cmd2 outfile", 2);
		exit(ERROR_USAGE);
	}

	manager = init_manager(argc, argv, envp);

	ft_pipex(manager);
	// Close all pipes in parent process
	close_pipes(manager);
	wait_for_children(manager, manager->n_cmds);
	return (0);
}