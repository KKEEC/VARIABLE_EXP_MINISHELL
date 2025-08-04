#include "../../../includes/tokenizer.h"

void    handle_special(const char *input, size_t *i, t_token **tokens)
{
    if (input[*i] == '|')
        add_token(tokens, create_token(TOKEN_PIPE, "|"));
    else if (input[*i] == '<' && input[*i + 1] == '<')
    {
        add_token(tokens, create_token(TOKEN_HEREDOC, "<<"));
        (*i)++;
    }
    else if (input[*i] == '>' && input[*i + 1] == '>')
    {
        add_token(tokens, create_token(TOKEN_APPEND, ">>"));
        (*i)++;
    }
    else if (input[*i] == '<')
        add_token(tokens, create_token(TOKEN_REDIR_IN, "<"));
    else if (input[*i] == '>')
        add_token(tokens, create_token(TOKEN_REDIR_OUT, ">"));
}
