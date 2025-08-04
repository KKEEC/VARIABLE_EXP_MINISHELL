#include "../../includes/env.h"

t_env *new_env_node(char *key, char *value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return NULL;
    node->key = ft_strdup(key);
    if (!value)
        node->value = NULL;
    else
        node->value = ft_strdup(value);
    node->next = NULL;
    return node;
}

void add_env_node(t_env **env_list, t_env *new_node)
{
    if (!*env_list)
        *env_list = new_node;
    else
    {
        t_env *temp = *env_list;
        while (temp->next)
            temp = temp->next;
        temp->next = new_node;
    }
}

t_env *init_env_list(char **envp)
{
    t_env   *env_list;
    t_env   *new_node;
    int i;
    char *equal;
    size_t key_len;
    char *key;
    char *value;

    env_list = NULL;
    i = 0;
    while(envp[i])
    {
        equal = ft_strchr(envp[i], '=');
        if (!equal)
        {
            i++;
            continue ;
        }
        key_len = equal - envp[i];
        key = ft_strndup(envp[i], key_len);
        value = ft_strdup(equal + 1);
        new_node = new_env_node(key, value);
        free(key);
        free(value);
        add_env_node(&env_list, new_node);
        i++;
    }
    return (env_list);

}