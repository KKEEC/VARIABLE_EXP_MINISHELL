#include "../../../includes/executor.h"

int execute_redirin(t_ast *ast, t_env **env_list)
{
    int stdin_save = -1, stdout_save = -1;
    int status = 0;
    t_ast *command_node;
    t_redir_info redirections[64];
    int redir_count = 0;
    int input_fd = -1;
    int output_fd = -1;
    int input_redirected = 0;
    int output_redirected = 0;

    // Find the actual command node
    command_node = find_command_node_input(ast);
    
    // Collect all redirections in the AST
    collect_all_redirections(ast, redirections, &redir_count);
    
    // Process all redirections in left-to-right order
    if (process_redirections_in_order_with_input(redirections, redir_count, &output_fd, &input_fd) != 0)
        return (1);
    
    // Set up input redirection if needed
    if (input_fd != -1)
    {
        stdin_save = dup(STDIN_FILENO);
        if (stdin_save == -1)
            return (perror("minishell: dup"), close(input_fd), 1);
        if (dup2(input_fd, STDIN_FILENO) == -1)
            return (close(input_fd), close(stdin_save), perror("minishell: dup2"), 1);
        close(input_fd);
        input_redirected = 1;
    }
    
    // Set up output redirection if needed
    if (output_fd != -1)
    {
        stdout_save = dup(STDOUT_FILENO);
        if (stdout_save == -1)
            return (perror("minishell: dup"), close(output_fd), 1);
        if (dup2(output_fd, STDOUT_FILENO) == -1)
            return (close(output_fd), close(stdout_save), perror("minishell: dup2"), 1);
        close(output_fd);
        output_redirected = 1;
    }
    
    // Execute the actual command
    status = execute_ast(command_node->left, env_list);
    
    // Restore file descriptors
    if (input_redirected)
    {
        if (dup2(stdin_save, STDIN_FILENO) == -1)
            return (perror("minishell: dup2 restore"), close(stdin_save), 1);
        close(stdin_save);
    }
    if (output_redirected)
    {
        if (dup2(stdout_save, STDOUT_FILENO) == -1)
            return (perror("minishell: dup2 restore"), close(stdout_save), 1);
        close(stdout_save);
    }
    
    return (status);
}
