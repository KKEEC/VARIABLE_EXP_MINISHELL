#include "../../../includes/executor.h"

int execute_redirin(t_ast *ast, t_env **env_list)
{
    (void)ast;
    (void)env_list;
    int fd;
    int stdin_save;
    int status = 0;

    fd = open(ast->filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Input redirection Error ");
        return (1);
    }
    stdin_save = dup(STDERR_FILENO);
    if (stdin_save == -1)
    {
        perror("minishell : dup");
        return (1);
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("minishell: dup2 redir in ");
        close(fd);
        close(stdin_save);
        return (1);
    }
    close(fd);
    status = execute_ast(ast->left, env_list);
    if (dup2(stdin_save, STDIN_FILENO) == -1)
    {
        perror("minishell: dup2 restore");
        return (1);
    }
    close(stdin_save);
    return (status);
}
