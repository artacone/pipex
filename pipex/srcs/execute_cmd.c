#include "../includes/pipex.h"

static char	**get_path(char *envp[])
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
				handle_error(ERROR_ALLOC, NULL);
			}
			break ;
		}
		i++;
	}
	return (path);
}

static char	*get_cmd_path(char *envp[], char *cmd)
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
			handle_error(ERROR_ALLOC, NULL);
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

	if (manager->is_here_doc)
		i++;
	close_pipes(manager->pipes, manager->n_pipes);
	cmd = ft_split((manager->argv)[i + 2], ' ');
	if (cmd == NULL)
	{
		handle_error(ERROR_ALLOC, NULL);
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
	handle_error(ERROR_EXEC, cmd[0]);
}

void	execute_cmd1(t_manager *manager)
{
	int	fd_in;
	int	**pipes;

	pipes = manager->pipes;
	if (manager->is_here_doc)
	{
		make_here_doc(manager);
		fd_in = open(manager->infile, O_RDONLY);
		unlink(manager->infile);
	}
	else
	{
		fd_in = open(manager->infile, O_RDONLY);
	}
	if (fd_in == -1)
	{
		handle_error(ERROR_OPEN, manager->infile);
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

	if (manager->is_here_doc)
		fd_out = open(manager->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		fd_out = open(manager->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd_out == -1)
	{
		handle_error(ERROR_OPEN, manager->outfile);
	}
	pipes = manager->pipes;
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	dup2(pipes[manager->n_pipes - 1][0], STDIN_FILENO);
	execute_cmd(manager, manager->n_cmds - 1);
}
