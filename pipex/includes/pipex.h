#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include <stdio.h>
# include "../libft/includes/libft.h"

# define ERROR_USAGE	1
# define ERROR_PIPE		2
# define ERROR_ALLOC	3
# define ERROR_FORK		4
# define ERROR_OPEN		5
# define ERROR_EXEC		6

# define USAGE	"usage: ./pipex [here_doc LIMITER]/[infile] cmd1 cmd2 outfile"

typedef struct s_manager
{
	int		is_here_doc;
	char	**argv;
	char	**envp;
	char	*infile;
	char	*outfile;
	char	*limiter;
	int		n_cmds;
	int		n_pipes;
	pid_t	*pids;
	int		**pipes;
}	t_manager;

void	handle_error(int error_code, char *str);

void	close_pipes(int **pipes, int n_pipes);
void	create_pipes(t_manager *manager);

void	execute_cmd1(t_manager *manager);
void	execute_cmd(t_manager *manager, int i);
void	execute_cmdn(t_manager *manager);

void	make_here_doc(t_manager *manager);

#endif