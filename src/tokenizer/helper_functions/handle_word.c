#include "../../../includes/tokenizer.h"

int    handle_word(const char *input, size_t *i, t_token **tokens)
{
    size_t  start = *i;
    char *raw;
    char *word;
    
    while (input[*i] && !ft_isspace(input[*i])
        && !ft_isschar(input[*i]) && input[*i] != '\'' && input[*i] != '"')
    {
        if (input[*i] == '\\' && input[*i + 1])
            (*i) += 2;
        else
            (*i)++;
    }
    raw = ft_strndup(&input[start], *i - start);
    if (!raw)
        return (1);
    word = handle_unquoted_esc_chars(raw, *i - start);
    free(raw);
    if (!word)
        return (1);
    add_token(tokens, create_token(TOKEN_WORD, word));
    free(word);
    (*i)--;
    return 0;
}
