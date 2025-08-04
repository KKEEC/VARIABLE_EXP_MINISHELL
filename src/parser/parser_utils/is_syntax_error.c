#include "../../../includes/parser.h"


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
        printf("syntax error near unexpected token `|'\n");
        return 1;
    }
    while (token && token->next)
    {
        if (is_operator(token->type))
        {
            if (token->next->type != TOKEN_WORD)
            {
                if (token->next->value)
                    printf("syntax error near unexpected token `%s'\n", token->next->value);
                else
                    printf("syntax error near unexpected token `newline'\n");
                return (1);

            }
        }
        token = token->next;
    }
    if (token && is_operator(token->type))
    {
        printf("syntax error near unexpected token 'newline'\n");
        return 1;
    }
    return 0;
}