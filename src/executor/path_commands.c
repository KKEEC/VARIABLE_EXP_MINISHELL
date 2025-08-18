/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkeec <kkeec@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 00:00:00 by kkeec             #+#    #+#             */
/*   Updated: 2025/08/18 00:00:00 by kkeec            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

static void	execute_path_command(t_ast *ast, char **envp)
{
	if (access(ast->args[0], F_OK) == 0)
	{
		if (access(ast->args[0], X_OK) != 0)
			handle_exec_error(ast->args[0], envp, 126, "Permission denied");
	}
	execve(ast->args[0], ast->args, envp);
	if (access(ast->args[0], F_OK) == 0)
	{
		if (access(ast->args[0], X_OK) != 0)
			handle_exec_error(ast->args[0], envp, 126, "Permission denied");
		else
			handle_exec_error(ast->args[0], envp, 126, "Is a directory");
	}
	else
		handle_exec_error(ast->args[0], envp, 127, "No such file or directory");
}

static void	execute_system_command(t_ast *ast, t_env *env_list, char **envp)
{
	char	*path;
	char	*full_path;

	path = get_env_value(env_list, "PATH");
	if (!path)
	{
		ft_printstderr("PATH not found\n");
		exit(127);
	}
	full_path = get_command_full_path(path, ast->args[0]);
	if (!full_path)
		handle_exec_error(ast->args[0], envp, 127, "command not found");
	execve(full_path, ast->args, envp);
	free(full_path);
	handle_exec_error(ast->args[0], envp, 127, "command not found");
}

static void	execute_child(t_ast *ast, t_env *env_list)
{
	char	**envp;

	envp = env_list_to_array(env_list);
	if (is_path_command(ast->args[0]))
		execute_path_command(ast, envp);
	else
		execute_system_command(ast, env_list, envp);
}

int	execute_commands(t_ast *ast, t_env **env_list)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		execute_child(ast, *env_list);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
