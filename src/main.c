#include "../includes/minishell.h"
#include "../includes/env.h"

int	main(int argc, char **argv, char **envp)
{
	char	*cwd;
	t_env	*env_list;
	int		status;

	(void)argc;
	(void)argv;
	env_list = NULL;
	status = 0;
	env_list = init_env_list(envp, &env_list);
	if (!get_env_value(env_list, "PWD"))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			update_env(&env_list, "PWD", cwd);
			free(cwd);
		}
	}
	minishell_loop(env_list, &status);
	if (env_list)
		free_env_list(env_list);
	rl_clear_history();
	return (status);
}
