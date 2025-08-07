#include "../../includes/executor.h"
#include "../../includes/minishell.h"
# include <fcntl.h>

inline
static int check_first_pid(pid_t pid, int *pipe_fd, t_ast *ast, t_env **env_list)
{
    if (pid < 0)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        perror("Fork error");
        return (1);
    }
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        close(pipe_fd[0]);
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
        close(pipe_fd[1]);
        exit(execute_ast(ast, env_list));
    }
    return (0);
}

inline
static int check_second_pid(pid_t pid, int *pipe_fd, t_ast *ast, t_env **env_list)
{
    if (pid < 0)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        perror("Fork error");
        return (1);
    }
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        close(pipe_fd[1]);
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
        {
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
        close(pipe_fd[0]);
        exit(execute_ast(ast, env_list));
    }
    return (0);
}

int execute_pipe(t_ast *ast, t_env **env_list)
{
    int pipe_fd[2];
    int status1;
    int status2;
    pid_t   pid1;
    pid_t   pid2;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe error");
        return (1);
    }
    pid1 = fork();
    if (check_first_pid(pid1, pipe_fd, ast->left, env_list) != 0)
        return (1);
    pid2 = fork();
    if (check_second_pid(pid2, pipe_fd, ast->right, env_list) != 0)
        return (1);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    signal(SIGINT, SIG_IGN);
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    if (WIFEXITED(status2))
        return (WEXITSTATUS(status2));
    else if (WIFSIGNALED(status2))
        return (128 + WTERMSIG(status2));
    
    return (g_status);
}


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

int execute_heredoc(t_ast *ast, t_env **env_list)
{
    (void)ast;
    (void)env_list;

    printf("Neeed to handle heredoc\n");
    return (0);
}


int is_builtin(const char *cmd)
{
    if (isstrequal(cmd, "echo") 
    || isstrequal(cmd, "cd") 
    || isstrequal(cmd, "pwd") 
    || isstrequal(cmd, "export") 
    || isstrequal(cmd, "unset")
    || isstrequal(cmd, "env")
    || isstrequal(cmd, "exit"))
        return (1);
    else
        return (0);
}

int execute_builtin(t_ast *node, t_env **env_list)
{
    char **args;
    int status;

    args = node->args;
    status = 0;
    if (isstrequal(args[0], "echo"))
        status = builtin_echo(args);
    else if (isstrequal(args[0], "pwd"))
        status = builtin_pwd();
    else if (isstrequal(args[0], "cd"))
        status = builtin_cd(args, *env_list);
    else if (isstrequal(args[0], "env"))
        status = builtin_env(*env_list, args);
    else if (isstrequal(args[0], "export"))
        status = builtin_export(args, env_list);
    else if (isstrequal(args[0], "unset"))
        status = builtin_unset(args, env_list);
    else if (isstrequal(args[0], "exit"))
        status = builtin_exit(args); 
    return (status);
}

int execute_ast(t_ast *ast, t_env **env_list)
{
    int status;

    status = 0;
    if (!ast)
        return status;
    if (ast->type == NODE_COMMAND)
    {
        if (!ast->args || !ast->args[0])
            return 0;
        if (is_builtin(ast->args[0]))
            status = execute_builtin(ast, env_list);
        else
            status = execute_commands(ast, env_list);
    }
    else if (ast->type == NODE_REDIR && ast->redir_type == TOKEN_REDIR_IN)
        status = execute_redirin(ast, env_list);
    else if (ast->type == NODE_REDIR && ast->redir_type == TOKEN_REDIR_OUT)
        status = execute_redirout(ast, env_list);
    else if (ast->type == NODE_REDIR && ast->redir_type == TOKEN_APPEND)
        status = execute_append(ast, env_list);
    else if (ast->type == NODE_REDIR && ast->redir_type == TOKEN_HEREDOC)
        status = execute_heredoc(ast, env_list);
    else if (ast->type == NODE_PIPE)
        status = execute_pipe(ast, env_list);
    return status;
}