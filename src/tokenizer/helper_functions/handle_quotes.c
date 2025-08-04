#include "../../../includes/tokenizer.h"

///maybe we dont need to check esxaped character backslash required by the subject

static int isescaped(const char *input, size_t i)
{
    int backslash;

    backslash = 0;
    while(i > 0 && input[--i] == '\\')
        backslash++;
    if (backslash % 2 == 0)
        return (0);
    else
        return (1);
}

int handle_double_quote(const char *input, size_t *i, t_token **tokens, t_env *env_list)
{
    printf("input from double quotes: %s\n", input);
    int start;
    char *word;
    //char *full_phrase;
    start = ++(*i);
    while (input[(*i)])
    {
        if (input[*i] == '"' && isescaped(input, *i) == 0)
            break ;
        (*i)++;
    }
    if (input[*i] != '"')
        return (0);
    printf("string from index opening quote: %s\n", &input[start]);
    word = ft_strndup(&input[start], *i - start);
    if (!word)
        return (printf("Malloc failed"), 0);
    printf("WORD: %s?\n", word);
    //free(word);
    char *expanded_val = expanddollar(word, env_list);
    if (expanded_val)
    {
        add_token(tokens, create_token(TOKEN_WORD, expanded_val));
        free(expanded_val);
    }

    (*i)++;
    return (1);
}

int handle_single_quote(const char *input, size_t *i, t_token **tokens)
{
    int start;
    char *word;

    start = ++(*i);
    while (input[*i] && input[*i] != '\'')
        (*i)++;
    if (input[*i] != '\'')  //descriptive message quote not closed
        return (0);
    word = ft_strndup(&input[start], *i - start);
    add_token(tokens, create_token(TOKEN_WORD, word));
    free(word);
    (*i)++;
    return (1);
}
