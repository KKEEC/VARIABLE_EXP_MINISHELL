#include "../../../includes/executor.h"

static int	setup_output_redirection(int final_fd, int *save_stdout)
{
	*save_stdout = dup(STDOUT_FILENO);
	if (*save_stdout == -1)
	{
		perror("minishell: dup");
		close(final_fd);
		return (1);
	}
	if (dup2(final_fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(final_fd);
		close(*save_stdout);
		return (1);
	}
	close(final_fd);
	return (0);
}

static int	restore_stdout(int save_stdout)
{
	if (dup2(save_stdout, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 restore");
		close(save_stdout);
		return (1);
	}
	close(save_stdout);
	return (0);
}

int	execute_redirout(t_ast *ast, t_env **env_list)
{
	int				save_stdout;
	int				status;
	t_ast			*node_to_execute;
	t_redir_info	redirections[64];

	status = 0;
	node_to_execute = find_command_node_output(ast);
	collect_all_redirections(ast, redirections, &status);
	if (process_redirections_in_order_basic(redirections, status,
			&save_stdout) != 0)
		return (1);
	if (save_stdout != -1)
	{
		if (setup_output_redirection(save_stdout, &save_stdout) != 0)
			return (1);
		status = execute_ast(node_to_execute, env_list);
		if (restore_stdout(save_stdout) != 0)
			return (1);
	}
	else
		status = execute_ast(node_to_execute, env_list);
	return (status);
}
