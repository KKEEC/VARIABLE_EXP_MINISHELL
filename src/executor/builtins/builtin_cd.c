/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkeec <kkeec@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 00:00:00 by kkeec             #+#    #+#             */
/*   Updated: 2025/08/18 00:00:00 by kkeec            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/executor.h"

static char	*get_cd_path(char **args, t_env *env_list)
{
	char	*path;

	if (!args[1] || args[1][0] == '\0')
	{
		path = get_env_value(env_list, "HOME");
		if (!path)
		{
			ft_printstderr("cd: HOME not set\n");
			return (NULL);
		}
		return (path);
	}
	return (args[1]);
}

static char	*save_old_pwd(t_env *env_list)
{
	char	*old_env;
	char	*oldpath;

	old_env = get_env_value(env_list, "PWD");
	if (old_env)
		oldpath = ft_strdup(old_env);
	else
		oldpath = NULL;
	return (oldpath);
}

static int	change_directory(char *path, char *oldpath)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpath);
		return (1);
	}
	return (0);
}

static int	update_pwd_vars(t_env **env_list, char *oldpath)
{
	char	*newpath;

	newpath = getcwd(NULL, 0);
	if (!newpath)
	{
		perror("cd: getcwd");
		free(oldpath);
		return (1);
	}
	update_env(env_list, "PWD", newpath);
	if (oldpath)
		update_env(env_list, "OLDPWD", oldpath);
	free(newpath);
	return (0);
}

int	builtin_cd(char **args, t_env *env_list)
{
	char	*path;
	char	*oldpath;

	if (args[1] && args[2])
	{
		ft_printstderr("cd: too many arguments\n");
		return (1);
	}
	path = get_cd_path(args, env_list);
	if (!path)
		return (1);
	oldpath = save_old_pwd(env_list);
	if (change_directory(path, oldpath) != 0)
		return (1);
	if (update_pwd_vars(&env_list, oldpath) != 0)
		return (1);
	free(oldpath);
	return (0);
}
