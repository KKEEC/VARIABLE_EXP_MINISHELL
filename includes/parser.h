#ifndef PARSER_H
# define PARSER_H

#include "tokenizer.h"

typedef enum    e_node_type
{
    NODE_COMMAND,
    NODE_PIPE,
    NODE_REDIR,
} t_node_type;
typedef struct s_ast
{
    t_node_type    type;
    struct s_ast    *left;
    struct s_ast    *right;
    char    **args;
    t_token_type redir_type;
    char *filename;
}   t_ast;

t_ast   *parse_tokens(t_token *tokens);

t_ast *new_command_node(void);

int is_syntax_error(t_token *token);
int is_redirection(t_token_type type);

void print_ast(t_ast *node, int depth);
void    free_ast(t_ast *node);




# endif