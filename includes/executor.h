#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"
#include "parser.h"
#include "env.h"


int execute_ast(t_ast *ast, t_env **env_list);

int is_builtin(const char *cmd);

int execute_builtin(t_ast *node, t_env **env_list);
int execute_commands(t_ast *ast, t_env **env_list);


int builtin_echo(char **args);
int builtin_pwd(void);
int builtin_cd(char **args, t_env *env_list);
int builtin_env(t_env *env_list, char **args);
int builtin_export(char **args, t_env **env_list);
void  case_noargs_export(t_env *env_list);
int case_withargs_export(char **args, t_env **env_list);
int builtin_unset(char **args, t_env **env_list);
int builtin_exit(char **args);

int execute_redirin(t_ast *ast, t_env **env_list);
int execute_redirout(t_ast *ast, t_env **env_list);
int execute_pipe(t_ast *ast, t_env **env_list);
int execute_append(t_ast *ast, t_env **env_list);
int execute_heredoc(t_ast *ast, t_env **env_list);

int is_valid_key(const char *key);
char    *trimquotes(char *value);

#endif