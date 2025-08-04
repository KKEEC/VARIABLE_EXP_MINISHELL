// minishell/src/main.c
#include "../includes/minishell.h"
#include "../includes/env.h"

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    char    *cwd;
    t_env   *env_list;
    
    env_list = init_env_list(envp);
    if (!get_env_value(env_list, "PWD"))
    {
        cwd = getcwd(NULL, 0);
        if (cwd)
        {
            update_env(&env_list, "PWD", cwd);
            free(cwd);
        }
    }
    minishell_loop(env_list);
    return (env_list->last_ex_status);
}
