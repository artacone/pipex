#include "../includes/pipex.h"

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
