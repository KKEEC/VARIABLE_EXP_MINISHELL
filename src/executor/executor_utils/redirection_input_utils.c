#include "../../../includes/executor.h"

int	handle_input_file(t_redir_info *redirs, int i, int *error_occurred)
{
	int	fd;

	fd = open(redirs[i].filename, O_RDONLY);
	if (fd == -1 && !*error_occurred)
	{
		perror("Input redirection Error ");
		*error_occurred = 1;
	}
	else if (fd != -1)
		close(fd);
	return (0);
}

int	handle_output_file(t_redir_info *redirs, int i, int *final_fd)
{
	int	fd;

	fd = open(redirs[i].filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror("Output redirection Error "), 1);
	if (i == 0)
		*final_fd = fd;
	else
		close(fd);
	return (0);
}

int	handle_append_file(t_redir_info *redirs, int i, int *final_fd)
{
	int	fd;

	fd = open(redirs[i].filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (perror("Output redirection Error "), 1);
	if (i == 0)
		*final_fd = fd;
	else
		close(fd);
	return (0);
}

int	process_redirections_in_order_with_input(t_redir_info *redirs, int count,
		int *final_fd, int *input_fd)
{
	int	i;
	int	fd;

	if (process_redirections_in_order_basic(redirs, count, final_fd) == 1)
		return (1);
	i = count - 1;
	while (i >= 0)
	{
		if (redirs[i].type == TOKEN_REDIR_IN)
		{
			fd = open(redirs[i].filename, O_RDONLY);
			if (fd != -1)
			{
				if (i == 0)
					*input_fd = fd;
				else
					close(fd);
			}
		}
		i--;
	}
	return (0);
}
