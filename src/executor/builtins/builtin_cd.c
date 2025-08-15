#include "../../../includes/executor.h"

int	builtin_cd(char **args, t_env *env_list)
{
	char	*path;
	char	*oldpath;
	char	*newpath;
	char	*old_env;

	if (args[1] && args[2])
	{
		ft_printstderr("cd: too many arguments\n");
		return (1);
	}
	if (!args[1] || args[1][0] == '\0')
	{
		path = get_env_value(env_list, "HOME");
		if (!path)
		{
			ft_printstderr("cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = args[1];
	old_env = get_env_value(env_list, "PWD");
	if (old_env)
		oldpath = ft_strdup(old_env);
	else
		oldpath = NULL;
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpath);
		return (1);
	}
	newpath = getcwd(NULL, 0);
	if (!newpath)
	{
		perror("cd: getcwd");
		free(oldpath);
		return (1);
	}
	update_env(&env_list, "PWD", newpath);
	if (oldpath)
		update_env(&env_list, "OLDPWD", oldpath);
	free(newpath);
	return (0);
}
