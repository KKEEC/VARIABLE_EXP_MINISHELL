#include "../../../includes/executor.h"

int execute_append(t_ast *ast, t_env **env_list)
{
    int fd;
    int stdout_save;
    int status = 0;

    fd = open(ast->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("minishell: output redirection");
        return (1);
    }
    stdout_save = dup(STDOUT_FILENO);
    if (stdout_save == -1)
    {
        close(fd);
        perror("minishell: dup");
        return (1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        close(stdout_save);
        perror("minishell: dup2");
        return (1);
    }
    close(fd);
    status = execute_ast(ast->left, env_list);
    if (dup2(stdout_save, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2 restore");
        return (1);
    }
    close(stdout_save);
    return (status);
}
