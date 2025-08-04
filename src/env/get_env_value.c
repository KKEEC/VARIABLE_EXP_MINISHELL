#include "../../includes/env.h"

char *get_env_value(t_env *env, const char *key)
{
    while (env)
    {
        if (isstrequal(env->key, key))
            return (env->value);
        env = env->next;
    }
    return (NULL);
}
