#include "../../../includes/executor.h"

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

static int process_redirections_in_order(t_redir_info *redirs, int count, int *final_fd, int *input_fd)
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
                if (!error_occurred) // Only print error for the first failure
                {
                    perror("Input redirection Error ");
                    error_occurred = 1;
                }
                // Don't break - continue to process remaining output files
            }
            else
            {
                if (i == 0 && !error_occurred) // This is the final input redirection and no errors
                    *input_fd = fd;
                else
                    close(fd);
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
                    perror("Output redirection Error ");
                    return (1);
                }
                if (i == 0 && !error_occurred) // This is the final output redirection and no errors
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
                    perror("Output redirection Error ");
                    return (1);
                }
                if (i == 0 && !error_occurred) // This is the final output redirection and no errors
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
    t_ast *cur = ast;
    
    // Traverse down to find the actual command node
    while (cur->left && cur->left->type == NODE_REDIR &&
           cur->left->redir_type == TOKEN_REDIR_IN)
    {
        cur = cur->left;
    }
    return (cur);
}

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
    command_node = find_command_node(ast);
    
    // Collect all redirections in the AST
    collect_all_redirections(ast, redirections, &redir_count);
    
    // Process all redirections in left-to-right order
    if (process_redirections_in_order(redirections, redir_count, &output_fd, &input_fd) != 0)
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
