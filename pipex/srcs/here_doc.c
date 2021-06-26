#include "../includes/pipex.h"

void	make_here_doc(t_manager *manager)
{
	int		fd_here_doc;
	char	*line;

	fd_here_doc = open(manager->infile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd_here_doc == -1)
	{
		handle_error(ERROR_OPEN, manager->infile);
	}
	while (1)
	{
		if ((get_next_line(STDIN_FILENO, &line) <= 0)
			|| (ft_strcmp(line, manager->limiter) == 0))
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd_here_doc);
		free(line);
	}
	close(fd_here_doc);
}
