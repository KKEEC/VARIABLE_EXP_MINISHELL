#include "../../../includes/executor.h"
#include "../../../includes/env.h"

static int	is_valid_key_unset(const char *key)
{
	int	i;

	i = 0;
	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_unset(char **args, t_env **env_list)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (args[i])
	{
		if (!is_valid_key_unset(args[i]))
		{
			ft_printstderr("unset: `");
			ft_printstderr(args[i]);
			ft_printstderr("`: not a valid identifier\n");
			status = 1;
		}
		else
			remove_env_value(env_list, args[i]);
		i++;
	}
	return (status);
}
