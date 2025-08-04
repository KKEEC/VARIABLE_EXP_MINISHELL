#include "../../../includes/executor.h"

int    builtin_env(t_env *env_list, char **args)
{
    if (args[1])
    {
        printf("env: too many arguments\n");
        return (1);
    }
    while (env_list)
    {
        if (env_list->value)
            printf("%s=%s\n", env_list->key, env_list->value);
        env_list = env_list->next;
    }
    return 0;
}