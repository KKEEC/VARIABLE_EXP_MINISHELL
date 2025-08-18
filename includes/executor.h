#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"
#include "parser.h"
#include "env.h"


int execute_ast(t_ast *ast, t_env **env_list);

int is_builtin(const char *cmd);

int execute_builtin(t_ast *node, t_env **env_list);
int execute_commands(t_ast *ast, t_env **env_list);

// String utilities
char	*ft_strncpyandjoin(char *path, int i, int j, char *cmd);
char	*ft_strjoin3(const char *s1, const char *s2, const char *s3);

// Environment utilities
void	free_envp(char **envp);
char	**env_list_to_array(t_env *env);

// Command utilities
char	*get_command_full_path(char *path, char *cmd);
void	handle_exec_error(const char *cmd, char **envp, int error_code, const char *error_msg);
int	is_path_command(const char *cmd);

int builtin_echo(char **args);
int builtin_pwd(void);
int builtin_cd(char **args, t_env *env_list);
int builtin_env(t_env *env_list, char **args);
int builtin_export(char **args, t_env **env_list);
void  case_noargs_export(t_env *env_list);
int case_withargs_export(char **args, t_env **env_list);
void	print_err_key(char *key);
int builtin_unset(char **args, t_env **env_list);
int builtin_exit(char **args);

int execute_redirin(t_ast *ast, t_env **env_list);
int execute_redirout(t_ast *ast, t_env **env_list);
int execute_pipe(t_ast *ast, t_env **env_list);
int execute_append(t_ast *ast, t_env **env_list);
int execute_heredoc(t_ast *ast, t_env **env_list);

int is_valid_key(const char *key);
char    *trimquotes(char *value);

// Shared redirection utilities
typedef struct s_redir_info {
    int type;
    char *filename;
    int order;
} t_redir_info;

int collect_all_redirections(t_ast *ast, t_redir_info *redirs, int *count);
int process_redirections_in_order_basic(t_redir_info *redirs, int count, int *final_fd);
int process_redirections_in_order_with_input(t_redir_info *redirs, int count, int *final_fd, int *input_fd);
t_ast *find_command_node_input(t_ast *ast);
t_ast *find_command_node_output(t_ast *ast);
int handle_input_file(t_redir_info *redirs, int i, int *error_occurred);
int handle_output_file(t_redir_info *redirs, int i, int *final_fd);
int handle_append_file(t_redir_info *redirs, int i, int *final_fd);

#endif