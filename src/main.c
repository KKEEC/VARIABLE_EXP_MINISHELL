// minishell/src/main.c
#include "../includes/minishell.h"
#include "../includes/env.h"

static int shell_status = 0;

void set_shell_status(int status)
{
    shell_status = status;
}

int get_shell_status(void)
{
    return shell_status;
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    char    *cwd;
    t_env   *env_list;
    env_list = NULL;
    int status;

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
