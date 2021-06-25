#include "../includes/pipex.h"

/*
 * TODO free stuff
 * bonus <<>>
 */

void	wait_for_children(pid_t *pids, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		waitpid(pids[i++], NULL, 0);
	}
}

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

void	create_pipes(t_manager *manager)
{
	int	**pipes;
	int	n_pipes;
	int	i;

	n_pipes = manager->n_pipes;
	pipes = (int **)malloc(n_pipes * sizeof(*pipes));
	if (pipes == NULL)
		handle_error(ERROR_ALLOC);
	i = 0;
	while (i < n_pipes)
	{
		pipes[i] = (int *)malloc(2 * sizeof(**pipes));
		if (pipes[i] == NULL)
			handle_error(ERROR_ALLOC);
		i++;
	}
	i = 0;
	while (i < n_pipes)
	{
		if (pipe(pipes[i]) == -1)
			handle_error(ERROR_PIPE);
		i++;
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
	manager->argv = argv;
	manager->envp = envp;
	manager->infile = argv[1];
	manager->outfile = argv[argc - 1];
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

void	close_pipes(int **pipes, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

char	**get_path(char *envp[])
{
	char	**path;
	int		i;

	path = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			path = ft_split(ft_strchr(envp[i], '=') + 1, ':');
			if (path == NULL)
			{
				handle_error(ERROR_ALLOC);
			}
			break ;
		}
		i++;
	}
	return (path);
}

char	*get_cmd_path(char *envp[], char *cmd)
{
	char	*cmd_path;
	char	**path;
	char	*to_free;
	int		i;

	path = get_path(envp);
	if (path == NULL)
		return (NULL);
	i = 0;
	while (path[i] != NULL)
	{
		cmd_path = ft_strjoin(path[i++], "/");
		to_free = cmd_path;
		cmd_path = ft_strjoin(cmd_path, cmd);
		if (cmd_path == NULL)
			handle_error(ERROR_ALLOC);
		free(to_free);
		if (access(cmd_path, X_OK) == 0)
			break ;
	}
	i = 0;
	while (path[i] != NULL)
		free(path[i++]);
	free(path);
	return (cmd_path);
}

void	execute_cmd(t_manager *manager, int i)
{
	char	**cmd;
	char	*cmd_path;

	close_pipes(manager->pipes, manager->n_pipes);
	cmd = ft_split((manager->argv)[i + 2], ' ');
	if (cmd == NULL)
	{
		handle_error(ERROR_ALLOC);
	}
	if ((ft_strncmp(cmd[0], "/", 1) == 0) || (ft_strncmp(cmd[0], ".", 1) == 0))
	{
		execve(cmd[0], cmd, manager->envp);
	}
	else
	{
		cmd_path = get_cmd_path(manager->envp, cmd[0]);
		execve(cmd_path, cmd, manager->envp);
	}
	handle_error_exec(cmd[0]);
}

void	execute_cmd1(t_manager *manager)
{
	int	fd_in;
	int	**pipes;

	pipes = manager->pipes;
	fd_in = open(manager->outfile, O_RDONLY);
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

	fd_out = open(manager->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
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
	int	i;

	i = -1;
	while (++i < manager->n_cmds)
	{
		(manager->pids)[i] = fork();
		if ((manager->pids)[i] == -1)
		{
			wait_for_children(manager->pids, i);
			handle_error(ERROR_FORK);
		}
		if ((manager->pids)[i] == 0)
		{
			if (i == 0)
				execute_cmd1(manager);
			else if (i == manager->n_cmds - 1)
				execute_cmdn(manager);
			else
			{
				dup2((manager->pipes)[i - 1][0], STDIN_FILENO);
				dup2((manager->pipes)[i][1], STDOUT_FILENO);
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
		ft_putendl_fd("usage: ./pipex infile cmd1 cmd2 outfile", 2);
		exit(ERROR_USAGE);
	}
	manager = init_manager(argc, argv, envp);
	ft_pipex(manager);
	close_pipes(manager->pipes, manager->n_pipes);
	wait_for_children(manager->pids, manager->n_cmds);
	return (0);
}
