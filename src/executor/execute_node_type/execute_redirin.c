#include "../../../includes/executor.h"

static int	setup_input_redir(int input_fd, int *stdin_save)
{
	*stdin_save = dup(STDIN_FILENO);
	if (*stdin_save == -1)
	{
		perror("minishell: dup");
		close(input_fd);
		return (1);
	}
	if (dup2(input_fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(input_fd);
		close(*stdin_save);
		return (1);
	}
	close(input_fd);
	return (0);
}

static int	setup_output_redir(int output_fd, int *stdout_save)
{
	*stdout_save = dup(STDOUT_FILENO);
	if (*stdout_save == -1)
	{
		perror("minishell: dup");
		close(output_fd);
		return (1);
	}
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(output_fd);
		close(*stdout_save);
		return (1);
	}
	close(output_fd);
	return (0);
}

static void	restore_fds(int stdin_save, int stdout_save, int input_redir,
			int output_redir)
{
	if (input_redir)
	{
		if (dup2(stdin_save, STDIN_FILENO) == -1)
			perror("minishell: dup2 restore");
		close(stdin_save);
	}
	if (output_redir)
	{
		if (dup2(stdout_save, STDOUT_FILENO) == -1)
			perror("minishell: dup2 restore");
		close(stdout_save);
	}
}

int	execute_redirin(t_ast *ast, t_env **env_list)
{
	int				fds[4];
	int				status;
	t_ast			*command_node;
	t_redir_info	redirections[64];

	fds[0] = -1;
	fds[1] = -1;
	fds[2] = -1;
	fds[3] = -1;
	status = 0;
	command_node = find_command_node_input(ast);
	collect_all_redirections(ast, redirections, &status);
	if (process_redirections_in_order_with_input(redirections, status,
			&fds[1], &fds[0]) != 0)
		return (1);
	if (fds[0] != -1 && setup_input_redir(fds[0], &fds[2]) != 0)
		return (1);
	if (fds[1] != -1 && setup_output_redir(fds[1], &fds[3]) != 0)
		return (1);
	status = execute_ast(command_node->left, env_list);
	restore_fds(fds[2], fds[3], (fds[2] != -1), (fds[3] != -1));
	return (status);
}
