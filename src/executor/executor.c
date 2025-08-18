#include "../../includes/executor.h"
#include "../../includes/minishell.h"
#include <stdio.h>

int	is_builtin(const char *cmd)
{
	if (isstrequal(cmd, "echo")
		|| isstrequal(cmd, "cd")
		|| isstrequal(cmd, "pwd")
		|| isstrequal(cmd, "export")
		|| isstrequal(cmd, "unset")
		|| isstrequal(cmd, "env")
		|| isstrequal(cmd, "exit"))
		return (1);
	else
		return (0);
}

int	execute_builtin(t_ast *node, t_env **env_list)
{
	char	**args;
	int		status;

	args = node->args;
	status = 0;
	if (isstrequal(args[0], "echo"))
		status = builtin_echo(args);
	else if (isstrequal(args[0], "pwd"))
		status = builtin_pwd();
	else if (isstrequal(args[0], "cd"))
		status = builtin_cd(args, *env_list);
	else if (isstrequal(args[0], "env"))
		status = builtin_env(*env_list, args);
	else if (isstrequal(args[0], "export"))
		status = builtin_export(args, env_list);
	else if (isstrequal(args[0], "unset"))
		status = builtin_unset(args, env_list);
	else if (isstrequal(args[0], "exit"))
		status = builtin_exit(args);
	return (status);
}

void	execute_command_node(t_ast *ast, t_env **env_list, int *status)
{
	if (is_builtin(ast->args[0]))
		*status = execute_builtin(ast, env_list);
	else
		*status = execute_commands(ast, env_list);
}

int	execute_ast(t_ast *ast, t_env **env_list)
{
	int	status;

	status = 0;
	if (!ast)
		return (status);
	if (ast->type == NODE_COMMAND)
	{
		if (!ast->args || !ast->args[0])
			return (0);
		execute_command_node(ast, env_list, &status);
	}
	else if (ast->type == NODE_REDIR && ast->redir_type == TOKEN_REDIR_IN)
		status = execute_redirin(ast, env_list);
	else if (ast->type == NODE_REDIR && ast->redir_type == TOKEN_REDIR_OUT)
		status = execute_redirout(ast, env_list);
	else if (ast->type == NODE_REDIR && ast->redir_type == TOKEN_APPEND)
		status = execute_append(ast, env_list);
	else if (ast->type == NODE_REDIR && ast->redir_type == TOKEN_HEREDOC)
		status = execute_heredoc(ast, env_list);
	else if (ast->type == NODE_PIPE)
		status = execute_pipe(ast, env_list);
	else
		status = 1;
	return (status);
}
