#include "../../../../includes/executor.h"

char	*extract_key(char *str)
{
	char	*key;
	char	*equal;
	char	*plus_equal;

	key = NULL;
	equal = ft_strchr(str, '=');
	plus_equal = ft_strnstr(str, "+=", ft_strlen(str));
	if (plus_equal)
		key = ft_substr(str, 0, plus_equal - str);
	else if (equal)
		key = ft_substr(str, 0, equal - str);
	else
		key = ft_strdup(str);
	return (key);
}

void	handle_plus_equal(char *plus_equal, char *key, t_env **env_list)
{
	char	*value;
	char	*trimval;
	char	*existing;
	char	*newval;

	value = ft_strdup(plus_equal + 2);
	trimval = trimquotes(value);
	existing = get_env_value(*env_list, key);
	if (existing)
	{
		newval = ft_strjoin(existing, trimval);
		update_env(env_list, key, newval);
		free(newval);
	}
	else
		update_env(env_list, key, trimval);
	free(value);
}

void	handle_equal(char *equal, char *key, t_env **env_list)
{
	char	*value;
	char	*newval;

	value = ft_strdup(equal + 1);
	newval = trimquotes(value);
	update_env(env_list, key, newval);
	free(value);
}

void	update_key_val(char *str, char *key, t_env **env_list)
{
	char	*equal;
	char	*plus_equal;

	equal = ft_strchr(str, '=');
	plus_equal = ft_strnstr(str, "+=", ft_strlen(str));
	if (plus_equal)
	{
		handle_plus_equal(plus_equal, key, env_list);
	}
	else if (equal)
		handle_equal(equal, key, env_list);
	else
	{
		if (!get_env_value(*env_list, str))
			update_env(env_list, str, NULL);
	}
}

int	case_withargs_export(char **args, t_env **env_list)
{
	int		i;
	char	*key;
	int		status;

	status = 0;
	i = 1;
	while (args[i])
	{
		key = NULL;
		key = extract_key(args[i]);
		if (!is_valid_key(args[i]))
		{
			print_err_key(args[i]);
			status = 1;
			free(key);
			i++;
			continue ;
		}
		update_key_val(args[i], key, env_list);
		free(key);
		i++;
	}
	return (status);
}
