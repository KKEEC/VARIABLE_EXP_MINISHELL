#include "../../../includes/parser.h"

int is_redirection(t_token_type type)
{
    if (type == TOKEN_REDIR_IN 
        || type == TOKEN_REDIR_OUT 
        || type == TOKEN_APPEND 
        || type == TOKEN_HEREDOC)
        return (1);
    return (0);
}