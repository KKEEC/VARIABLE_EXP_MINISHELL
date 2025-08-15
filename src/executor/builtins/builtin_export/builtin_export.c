#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"

int	builtin_export(char **args, t_env **env_list)
{
	if (!args[1])
	{
		case_noargs_export(*env_list);
		return (0);
	}
	return (case_withargs_export(args, env_list));
}
