#include "../includes/pipex.h"

static void	wait_for_children(pid_t *pids, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		waitpid(pids[i++], NULL, 0);
	}
}

static t_manager	*init_manager(int argc, char *argv[], char *envp[])
{
	t_manager	*manager;

	manager = (t_manager *)malloc(sizeof(*manager));
	if (manager == NULL)
		handle_error(ERROR_ALLOC, NULL);
	if (ft_strcmp(argv[1], "here_doc") == 0)
		manager->is_here_doc = 1;
	else
		manager->is_here_doc = 0;
	manager->argv = argv;
	manager->envp = envp;
	if (manager->is_here_doc)
		manager->n_cmds = argc - 4;
	else
		manager->n_cmds = argc - 3;
	manager->infile = argv[1];
	manager->outfile = argv[argc - 1];
	manager->limiter = argv[2];
	manager->n_pipes = manager->n_cmds - 1;
	manager->pids = (pid_t *)malloc(manager->n_cmds * sizeof(pid_t));
	if (manager->pids == NULL)
		handle_error(ERROR_ALLOC, NULL);
	create_pipes(manager);
	return (manager);
}

static void	ft_pipex(t_manager *manager)
{
	int	i;

	i = -1;
	while (++i < manager->n_cmds)
	{
		(manager->pids)[i] = fork();
		if ((manager->pids)[i] == -1)
		{
			wait_for_children(manager->pids, i);
			handle_error(ERROR_FORK, NULL);
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

static void	free_manager(t_manager *manager)
{
	int	i;

	i = 0;
	while (i < manager->n_pipes)
	{
		free(manager->pipes[i]);
		i++;
	}
	free(manager->pipes);
	free(manager->pids);
	free(manager);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_manager	*manager;

	if (argc < 5)
	{
		ft_putendl_fd(USAGE, 2);
		exit(ERROR_USAGE);
	}
	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		if (argc < 6)
		{
			ft_putendl_fd(USAGE, 2);
			exit(ERROR_USAGE);
		}
	}
	manager = init_manager(argc, argv, envp);
	ft_pipex(manager);
	close_pipes(manager->pipes, manager->n_pipes);
	wait_for_children(manager->pids, manager->n_cmds);
	free_manager(manager);
	return (0);
}
