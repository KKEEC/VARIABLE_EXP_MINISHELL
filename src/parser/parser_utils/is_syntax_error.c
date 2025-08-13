#include "../../../includes/parser.h"
#include "../../../includes/utils.h"
#include <unistd.h>


static  int is_operator(t_token_type type)
{
    if (type == TOKEN_PIPE
        || type == TOKEN_REDIR_IN 
        || type == TOKEN_REDIR_OUT 
        || type == TOKEN_APPEND 
        || type == TOKEN_HEREDOC)
        return (1);
    return (0);
}


int is_syntax_error(t_token *token)
{
    if (!token)
        return (0);
    if (token->type == TOKEN_PIPE)
    {
        write(STDERR_FILENO, " syntax error near unexpected token `|'\n", 39);
        return 1;
    }
    while (token && token->next)
    {
        if (is_operator(token->type))
        {
            if (token->next->type != TOKEN_WORD)
            {
                // Special case handling for specific operator sequences
                if (token->type == TOKEN_PIPE && (token->next->type == TOKEN_REDIR_OUT || token->next->type == TOKEN_APPEND))
                {
                    // Case: | > filename cmd or | >> filename cmd
                    // This is valid if we have: | > filename word [more tokens]
                    t_token *check = token->next->next; // Should be filename
                    if (check && check->type == TOKEN_WORD)
                    {
                        check = check->next; // Should be command word
                        if (check && check->type == TOKEN_WORD)
                        {
                            // This is valid: | > filename command
                            // Skip to continue checking after the command
                            token = check;
                            continue;
                        }
                    }
                    // If we get here, it's invalid syntax
                    if (token->next->type == TOKEN_APPEND)
                        write(STDERR_FILENO, " syntax error near unexpected token `>>'\n", 40);
                    else
                        write(STDERR_FILENO, " syntax error near unexpected token `newline'\n", 45);
                }
                else if (token->type == TOKEN_PIPE && token->next->type == TOKEN_REDIR_IN)
                {
                    // Case: | < |
                    if (token->next->next && token->next->next->type == TOKEN_PIPE)
                        write(STDERR_FILENO, " syntax error near unexpected token `|'\n", 39);
                    else
                        write(STDERR_FILENO, " syntax error near unexpected token `<'\n", 39);
                }
                else if (token->next->value)
                {
                    write(STDERR_FILENO, " syntax error near unexpected token ", 36);
                    write(STDERR_FILENO, "`", 1);
                    write(STDERR_FILENO, token->next->value, ft_strlen(token->next->value));
                    write(STDERR_FILENO, "'\n", 2);
                }
                else
                    write(STDERR_FILENO, " syntax error near unexpected token `newline'\n", 45);
                return (1);

            }
        }
        token = token->next;
    }
    if (token && is_operator(token->type))
    {
        // Special case for redirection operators at end of input
        if (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT || token->type == TOKEN_APPEND)
            write(STDERR_FILENO, " syntax error near unexpected token `newline'\n", 45);
        else
            write(STDERR_FILENO, " syntax error near unexpected token `newline'\n", 45);
        return 1;
    }
    return 0;
}