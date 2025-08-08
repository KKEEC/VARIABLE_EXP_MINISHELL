#include "../../../includes/executor.h"

int execute_redirout(t_ast *ast, t_env **env_list)
{
    (void)ast;
    (void)env_list;
    int fd;
    int save_stdout;
    int status = 0;

    fd = open(ast->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("minishell: output redirection");
        return 1;
    }
    save_stdout = dup(STDOUT_FILENO);
    if (save_stdout == -1)
    {
        perror("minishell: dup");
        close(fd);
        return (1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        close(save_stdout);
        perror("minishell: dup2");
        return (1);
    }
    close(fd);
    status = execute_ast(ast->left, env_list);
    if (dup2(save_stdout, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2 restore");
        return (1);
    }
    close(save_stdout);
    return (status);
}
