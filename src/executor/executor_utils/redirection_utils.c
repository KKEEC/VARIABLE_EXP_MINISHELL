#include "../../../includes/executor.h"

int	collect_all_redirections(t_ast *ast, t_redir_info *redirs, int *count)
{
	t_ast	*current;

	current = ast;
	*count = 0;
	while (current && current->type == NODE_REDIR && *count < 64)
	{
		redirs[*count].type = current->redir_type;
		redirs[*count].filename = current->filename;
		redirs[*count].order = *count;
		(*count)++;
		current = current->left;
	}
	return (0);
}

static int	open_and_handle_file(t_redir_info *redirs, int i, int *final_fd,
			int *error_occurred)
{
	if (redirs[i].type == TOKEN_REDIR_IN)
		handle_input_file(redirs, i, error_occurred);
	else if (redirs[i].type == TOKEN_REDIR_OUT && !*error_occurred)
	{
		if (handle_output_file(redirs, i, final_fd) == 1)
			return (1);
	}
	else if (redirs[i].type == TOKEN_APPEND && !*error_occurred)
	{
		if (handle_append_file(redirs, i, final_fd) == 1)
			return (1);
	}
	return (0);
}

int	process_redirections_in_order_basic(t_redir_info *redirs, int count,
		int *final_fd)
{
	int	i;
	int	error_occurred;

	error_occurred = 0;
	i = count - 1;
	while (i >= 0)
	{
		if (open_and_handle_file(redirs, i, final_fd, &error_occurred) == 1)
			return (1);
		i--;
	}
	if (error_occurred)
		return (1);
	return (0);
}

t_ast	*find_command_node_input(t_ast *ast)
{
	t_ast	*cur;

	cur = ast;
	while (cur->left && cur->left->type == NODE_REDIR
		&& cur->left->redir_type == TOKEN_REDIR_IN)
	{
		cur = cur->left;
	}
	return (cur);
}

t_ast	*find_command_node_output(t_ast *ast)
{
	t_ast	*node_to_execute;

	node_to_execute = ast->left;
	while (node_to_execute && node_to_execute->type == NODE_REDIR
		&& (node_to_execute->redir_type == TOKEN_REDIR_OUT
			|| node_to_execute->redir_type == TOKEN_APPEND))
	{
		node_to_execute = node_to_execute->left;
	}
	return (node_to_execute);
}
