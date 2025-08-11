#include "../../../includes/executor.h"

static void create_redir_file(t_ast *ast)
{
    t_ast *node = ast;
    while (node && node->type == NODE_REDIR &&
        (node->redir_type == TOKEN_REDIR_OUT || node->redir_type == TOKEN_APPEND))
    {
        if (node != ast)
        {
            int tmp_fd = open(node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (tmp_fd != -1)
                close(tmp_fd);
        }
        node = node->left;
    }
}

static t_ast *find_left_redir(t_ast *ast)
{
    t_ast *cur = ast;
    while(cur->left && cur->left->type == NODE_REDIR &&
           (cur->left->redir_type == TOKEN_REDIR_OUT || cur->left->redir_type == TOKEN_APPEND)) {
        cur = cur->left;
    }
    return (cur);
}

int execute_redirout(t_ast *ast, t_env **env_list)
{
    int fd;
    int save_stdout;
    int status;
    t_ast *cur;

    fd = -1;
    cur = find_left_redir(ast);
    create_redir_file(ast);
    fd = open(ast->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return (perror("minishell: output redirection"), 1);
    save_stdout = dup(STDOUT_FILENO);
    if (save_stdout == -1)
        return (perror("minishell: dup"), close(fd), 1);
    if (dup2(fd, STDOUT_FILENO) == -1)
        return (close(fd), close(save_stdout), perror("minishell: dup2"), 1);
    close(fd);
    status = execute_ast(cur->left, env_list);
    if (dup2(save_stdout, STDOUT_FILENO) == -1)
        return (perror("minishell: dup2 restore"), close(save_stdout), 1);
    close(save_stdout);
    return (status);
}
