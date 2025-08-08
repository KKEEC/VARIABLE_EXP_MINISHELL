#include "../../../../includes/executor.h"

int case_withargs_export(char **args, t_env **env_list)
{
    int i;
    char *key;
    char *value;
    char *equal_sign;
    char *existing;
    char *plus_equal;
    int status = 0;

    i = 1;
    while (args[i])
    {
        key = NULL;
        value = NULL;
        equal_sign = ft_strchr(args[i], '=');
        plus_equal = ft_strnstr(args[i], "+=", ft_strlen(args[i]));
        if (plus_equal)
            key = ft_substr(args[i], 0, plus_equal - args[i]);
        else if (equal_sign)
            key = ft_substr(args[i], 0, equal_sign - args[i]);
        else
            key = ft_strdup(args[i]);
        
        if (!is_valid_key(args[i]))
        {
            ft_printstderr("export: `");
            ft_printstderr(key);
            ft_printstderr("`: not a valid identifier\n");
            status = 1;
            free(key);
            i++;
            continue ;
        }
        if (plus_equal)
        {
            value = ft_strdup(plus_equal + 2);
            char *newval = trimquotes(value);
            existing = get_env_value(*env_list, key);
            if (existing)
            {
                char *new_value = ft_strjoin(existing, newval);
                update_env(env_list, key, new_value);
                free(new_value);
            }
            else
                update_env(env_list, key, newval);
            free(value);
        }
        else if (equal_sign)
        {
            
            value = ft_strdup(equal_sign + 1);
            char *newval = trimquotes(value);
            update_env(env_list, key, newval);
            free(value);
        }
        else
        {
            if (!get_env_value(*env_list, args[i]))
                update_env(env_list, args[i], NULL);
        }
        free(key);
        i++;
    }
    return (status);
}