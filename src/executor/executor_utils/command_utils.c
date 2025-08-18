/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkeec <kkeec@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 00:00:00 by kkeec             #+#    #+#             */
/*   Updated: 2025/08/18 00:00:00 by kkeec            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/executor.h"

char	*get_command_full_path(char *path, char *cmd)
{
	int		start_idx;
	int		end_idx;
	char	*full_path;

	start_idx = 0;
	end_idx = 0;
	while (path[end_idx])
	{
		if (path[end_idx] == ':' || path[end_idx + 1] == '\0')
		{
			if (path[end_idx + 1] == '\0')
				end_idx++;
			full_path = ft_strncpyandjoin(path, start_idx, end_idx, cmd);
			if (access(full_path, X_OK) == 0)
				return (full_path);
			free(full_path);
			start_idx = end_idx + 1;
		}
		end_idx++;
	}
	return (NULL);
}

void	handle_exec_error(const char *cmd,
		char **envp,
		int error_code,
		const char *error_msg)
{
	ft_printstderr("minishell: ");
	ft_printstderr(cmd);
	ft_printstderr(": ");
	ft_printstderr(error_msg);
	ft_printstderr("\n");
	if (envp)
		free_envp(envp);
	exit(error_code);
}

int	is_path_command(const char *cmd)
{
	if (!cmd)
		return (0);
	if (cmd[0] == '/')
		return (1);
	if (cmd[0] == '.' && cmd[1] == '/')
		return (1);
	if (cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '/')
		return (1);
	return (0);
}
