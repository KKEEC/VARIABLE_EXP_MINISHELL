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

char *handle_double_quote(const char *input, size_t *i, t_env *env_list)
{
    int start;
    char *word;
    char *expanded_val;

    start = ++(*i);
    while (input[(*i)])
    {
        if (input[*i] == '"' && !isescaped(input, *i))
            break ;
        (*i)++;
    }
    if (input[*i] != '"')
        return (NULL);
    word = ft_strndup(&input[start], *i - start);
    (*i)++;
    if (!word)
        return (NULL);
    expanded_val = expanddollar(word, env_list);
    free(word);
    return (expanded_val);
}

char *handle_single_quote(const char *input, size_t *i)
{
    int start;
    char *word;

    start = ++(*i);
    while (input[*i] && input[*i] != '\'')
        (*i)++;
    if (input[*i] != '\'')  //descriptive message quote not closed
        return (NULL);
    word = ft_strndup(&input[start], *i - start);
    (*i)++;
    if (!word)
        return (NULL);
    return (word);
}
