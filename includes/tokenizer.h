#ifndef TOKENIZER_H
# define TOKENIZER_H

#include <stdio.h>
# include "utils.h"
# include "env.h"

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_EOF
}   t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token  *next;
}   t_token;

// tokenizer.c
t_token *tokenize(const char *input, t_env *env_list);

// utils/
void    free_tokens(t_token *head);
t_token *create_token(t_token_type type, const char *input);
void    add_token(t_token **head, t_token *new_token);

// helper functions/
char	*expanddollar(const char *str, t_env *env_list);
void   handle_special(const char *input, size_t *i, t_token **tokens);
int    handle_word(const char *input, size_t *i, t_token **tokens);
char    *handle_single_quote(const char *input, size_t *i);
char    *handle_double_quote(const char *input, size_t *i, t_env *env_list);
char *handle_quoted_esc_chars(const char *src, size_t len);
char *handle_unquoted_esc_chars(const char *src, size_t len);

# endif
