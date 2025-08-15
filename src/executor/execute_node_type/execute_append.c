#include "../../../includes/executor.h"
#include <unistd.h>

int execute_append(t_ast *ast, t_env **env_list)
{
    int save_stdout;
    int status;
    t_ast *node_to_execute;
    t_redir_info redirections[64];
    int redir_count = 0;
    int final_fd = -1;

    // Find the actual command node
    node_to_execute = find_command_node_output(ast);
    
    // Collect all redirections in the AST
    collect_all_redirections(ast, redirections, &redir_count);
    
    // Process all redirections in left-to-right order
    if (process_redirections_in_order_basic(redirections, redir_count, &final_fd) != 0)
        return (1);
    
    // If we have a final output redirection, set it up
    if (final_fd != -1)
    {
        save_stdout = dup(STDOUT_FILENO);
        if (save_stdout == -1)
            return (perror("minishell: dup"), close(final_fd), 1);
        if (dup2(final_fd, STDOUT_FILENO) == -1)
            return (close(final_fd), close(save_stdout), perror("minishell: dup2"), 1);
        close(final_fd);
        
        // Execute the actual command
        status = execute_ast(node_to_execute, env_list);
        
        if (dup2(save_stdout, STDOUT_FILENO) == -1)
            return (perror("minishell: dup2 restore"), close(save_stdout), 1);
        close(save_stdout);
    }
    else
    {
        // No output redirection, execute normally
        status = execute_ast(node_to_execute, env_list);
    }
    
    return (status);
}
