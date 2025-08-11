#ifndef ENV_H
# define ENV_H

#include <stdio.h>
#include "utils.h"

typedef struct s_env
{
    char    *key;
    char    *value;
    struct  s_env   *next;
} t_env;

t_env *init_env_list(char **envp, t_env **env_list);
t_env *new_env_node(char *key, char *value);
void add_env_node(t_env **env_list, t_env *new_node);
char *get_env_value(t_env *env, const char *key);
void    remove_env_value(t_env **env_list, const char *key);
void  update_env(t_env **env_list, const char *key, const char *value);

int  env_list_size(t_env *env);
void free_env_list(t_env *env_list);

#endif