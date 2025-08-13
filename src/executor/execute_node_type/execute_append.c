#include "../../../includes/executor.h"
#include <unistd.h>

typedef struct s_redir_info {
    int type;
    char *filename;
    int order;
} t_redir_info;

static int collect_all_redirections(t_ast *ast, t_redir_info *redirs, int *count)
{
    t_ast *current = ast;
    *count = 0;
    
    // Collect all redirections with their order (reverse since AST is right-to-left)
    while (current && current->type == NODE_REDIR && *count < 64)
    {
        redirs[*count].type = current->redir_type;
        redirs[*count].filename = current->filename;
        redirs[*count].order = *count;
        (*count)++;
        current = current->left;
    }
    return (0);
}

static int process_redirections_in_order(t_redir_info *redirs, int count, int *final_fd)
{
    int i, fd;
    int error_occurred = 0;
    int first_input_index = -1;
    
    // Find the first input redirection index (in command line order)
    for (i = count - 1; i >= 0; i--)
    {
        if (redirs[i].type == TOKEN_REDIR_IN)
        {
            first_input_index = i;
            break;
        }
    }
    
    // Process redirections in reverse order (left-to-right from command line)
    for (i = count - 1; i >= 0; i--)
    {
        if (redirs[i].type == TOKEN_REDIR_IN)
        {
            fd = open(redirs[i].filename, O_RDONLY);
            if (fd == -1)
            {
                perror("minishell: input redirection");
                error_occurred = 1;
                // Don't break - continue to process remaining output files
            }
            else
            {
                close(fd); // We'll handle input redirection later
            }
        }
        else if (redirs[i].type == TOKEN_REDIR_OUT)
        {
            // Only create output files if:
            // 1. No input error occurred, OR
            // 2. This output redirection comes BEFORE the first input redirection in command line
            if (!error_occurred || (first_input_index != -1 && i > first_input_index))
            {
                fd = open(redirs[i].filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1)
                {
                    perror("minishell: output redirection");
                    return (1);
                }
                if (i == 0 && !error_occurred) // This is the final output redirection and no input errors
                    *final_fd = fd;
                else
                    close(fd); // Create the file but don't use for redirection
            }
        }
        else if (redirs[i].type == TOKEN_APPEND)
        {
            // Only create output files if:
            // 1. No input error occurred, OR
            // 2. This output redirection comes BEFORE the first input redirection in command line
            if (!error_occurred || (first_input_index != -1 && i > first_input_index))
            {
                fd = open(redirs[i].filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd == -1)
                {
                    perror("minishell: output redirection");
                    return (1);
                }
                if (i == 0 && !error_occurred) // This is the final output redirection and no input errors
                    *final_fd = fd;
                else
                    close(fd); // Create the file but don't use for redirection
            }
        }
    }
    
    return (error_occurred ? 1 : 0);
}

static t_ast *find_command_node(t_ast *ast)
{
    t_ast *node_to_execute;
    
    // Find the actual command node, skipping over nested output redirections
    node_to_execute = ast->left;
    while (node_to_execute && node_to_execute->type == NODE_REDIR && 
           (node_to_execute->redir_type == TOKEN_REDIR_OUT || 
            node_to_execute->redir_type == TOKEN_APPEND))
    {
        node_to_execute = node_to_execute->left;
    }
    return (node_to_execute);
}

int execute_append(t_ast *ast, t_env **env_list)
{
    int save_stdout;
    int status;
    t_ast *node_to_execute;
    t_redir_info redirections[64];
    int redir_count = 0;
    int final_fd = -1;

    // Find the actual command node
    node_to_execute = find_command_node(ast);
    
    // Collect all redirections in the AST
    collect_all_redirections(ast, redirections, &redir_count);
    
    // Process all redirections in left-to-right order
    if (process_redirections_in_order(redirections, redir_count, &final_fd) != 0)
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
