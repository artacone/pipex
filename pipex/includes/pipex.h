#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <stdio.h> // FIXME
# include "../libft/includes/libft.h"

# define ERROR_USAGE	1
# define ERROR_PIPE		2
# define ERROR_ALLOC	3
# define ERROR_FORK		4
# define ERROR_OPEN		5
# define ERROR_EXEC		6

typedef struct s_manager
{
	int		argc;
	char	**argv;
	char	**envp;
	int		n_cmds;
	int		n_pipes;
	pid_t	*pids;
	int		**pipes;
}	t_manager;

#endif