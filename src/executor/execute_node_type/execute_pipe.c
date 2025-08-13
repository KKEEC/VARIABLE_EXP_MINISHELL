#include "../../../includes/executor.h"

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
    
    // Check if the first process died from SIGPIPE
    if (WIFSIGNALED(status1) && WTERMSIG(status1) == SIGPIPE)
    {
        write(STDERR_FILENO, "Broken pipe\n", 12);
    }
    
    if (WIFEXITED(status2))
        return (WEXITSTATUS(status2));
    else if (WIFSIGNALED(status2))
        return (128 + WTERMSIG(status2)); 
    return (0);
}