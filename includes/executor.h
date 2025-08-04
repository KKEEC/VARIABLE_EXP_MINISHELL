#ifndef EXECUTOR_H
# define EXECUTOR_H


#include <unistd.h>
#include "minishell.h"
#include "parser.h"
#include "env.h"


void execute_ast(t_ast *ast, t_env **env_list);

int execute_command(t_ast *node);


int is_builtin(const char *cmd);

int execute_builtin(t_ast *node, t_env **env_list);


int builtin_echo(char **args);
int builtin_pwd(void);
int builtin_cd(char **args, t_env *env_list);
int    builtin_env(t_env *env_list, char **args);
int builtin_export(char **args, t_env **env_list);

void    setup_signalhandler(void);

#endif