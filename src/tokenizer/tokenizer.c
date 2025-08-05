#include "../../includes/tokenizer.h"
#include "../../includes/env.h"

char *strappend(char *s1, char *s2)
{
    char *res;

    if (!s1 && !s2)
        return NULL;
    if (!s1)
        return s2;
    if (!s2)
        return s1;

    res = ft_strjoin(s1, s2);
    free(s1);
    free(s2);
    return res;
}


static void	handle_spec_or_word(const char *input, size_t *i, t_token **tokens)
{
	if (ft_isschar(input[*i]))
		handle_special(input, i, tokens);
	else
		handle_word(input, i, tokens);
	(*i)++;
}

static char *handle_quotes(const char *input, size_t *i, t_env *env_list)
{
    if (input[*i] == '\'')
        return handle_single_quote(input, i); // no env needed
    else if (input[*i] == '"')
        return handle_double_quote(input, i, env_list);
    return NULL;
}


static char	*handle_normal_word(const char *input, size_t *i, t_env *env_list)
{
	size_t	start = *i;
	char	*token_val;
	char	quote;
	char *expanded_val;

	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '\'' && input[*i] != '"' && !ft_isschar(input[*i]))
    {
        if (input[*i] == '=' && (input[*i + 1] == '"' || input[*i + 1] == '\''))
        {
            quote = input[*i + 1];
            *i += 2;
            while (input[*i] && input[*i] != quote)
                (*i)++;
            if (input[*i] == quote)
                (*i)++;
            break;
        }
        (*i)++;
    }

    token_val = ft_strndup(&input[start], *i - start);
    expanded_val = expanddollar(token_val, env_list);
    free(token_val);
    return expanded_val;
}

t_token *tokenize(const char *input, t_env *env_list)
{
    t_token *tokens = NULL;
    size_t i = 0;
    char *acc = NULL; // buffer to accumulate parts of a word

    if (!input)
        return NULL;

    while (input[i])
    {
        if (ft_isspace(input[i]))
        {
            if (acc) {
                add_token(&tokens, create_token(TOKEN_WORD, acc));
                acc = NULL;
            }
            i++;
        }
        else if (!ft_isschar(input[i]) && input[i] != '\'' && input[i] != '"')
        {
            char *part = handle_normal_word(input, &i, env_list);
            acc = strappend(acc, part); // helper to join and free both
        }
        else if (input[i] == '\'' || input[i] == '"')
        {
            char *part = handle_quotes(input, &i, env_list);
            if (!part) {
                free(acc);
                free_tokens(tokens);
                return NULL;
            }
            acc = strappend(acc, part);
        }
        else
        {
            if (acc) {
                add_token(&tokens, create_token(TOKEN_WORD, acc));
                acc = NULL;
            }
            handle_spec_or_word(input, &i, &tokens);
        }
    }

    if (acc) // flush remaining word
        add_token(&tokens, create_token(TOKEN_WORD, acc));

    return tokens;
}

