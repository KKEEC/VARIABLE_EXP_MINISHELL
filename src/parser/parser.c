#include "../../includes/parser.h"
#include "../../includes/utils.h"

void print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("  ");
}

void print_ast(t_ast *node, int depth)
{
    if (!node)
        return;

    print_indent(depth);
    if (node->type == NODE_COMMAND)
    {
        printf("COMMAND: ");
        for (int i = 0; node->args && node->args[i]; i++)
            printf("%s ", node->args[i]);
        printf("\n");
    }
    else if (node->type == NODE_PIPE)
    {
        printf("PIPE\n");
        print_ast(node->left, depth + 1);
        print_ast(node->right, depth + 1);
    }
    else if (node->type == NODE_REDIR)
    {
        printf("REDIRECTION: ");
        if (node->redir_type == TOKEN_REDIR_IN)
            printf("< ");
        else if (node->redir_type == TOKEN_REDIR_OUT)
            printf("> ");
        else if (node->redir_type == TOKEN_APPEND)
            printf(">> ");
        else if (node->redir_type == TOKEN_HEREDOC)
            printf("<< ");
        printf("file: %s\n", node->filename);
        print_ast(node->left, depth + 1);
    }
}



///////////////////////////////////////////////////////////////

t_ast   *new_pipe_node(t_ast *left, t_ast *right)
{
    t_ast *node;

    node = malloc(sizeof(t_ast));
    if (!node)
        return (NULL);
    node->type = NODE_PIPE;
    node->left = left;
    node->right = right;
    node->args = NULL;
    node->redir_type = -1;
    node->filename = NULL;
    return (node);
}

int add_arg_to_command(t_ast *cmd, const char *arg)
{
    int count;
    char **new_args;

    count = 0;
    // count current args
    while (cmd->args && cmd->args[count])
        count++;
    //re allocate args with space for new arg + NULL terminator
    new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
        return (0);
    //copy existing args
    int i = 0;
    while (i < count)
    {
        new_args[i] = cmd->args[i];
        i++;
    }

    //add new arg
    new_args[i] = ft_strdup(arg);
    if (!new_args[i])
    {
        free(new_args);
        return (0);
    }

    new_args[i + 1] = NULL;

    free(cmd->args);
    cmd->args = new_args;
    return 1;
}

t_ast *new_redir_node(int redir_type, const char *filename, t_ast *cmd)
{
    t_ast *node;
    
    node = malloc(sizeof(t_ast));
    if (!node)
        return (NULL);
    node->type = NODE_REDIR;
    node->redir_type = redir_type;
    node->filename = ft_strdup(filename);
    node->left = cmd;
    node->right = NULL;
    node->args = NULL;

    return (node);
}

t_ast *new_command_node(void)
{
    t_ast *node;
    
    node = malloc(sizeof(t_ast));
    if (!node)
        return (NULL);
    node->type = NODE_COMMAND;
    node->left = NULL;
    node->right = NULL;
    node->args = NULL;
    node->redir_type = -1;
    node->filename = NULL;

    return (node);
}

t_ast   *parse_command_segment(t_token  **tokens)
{
    t_ast *cmd;
    int redir_type;
    char *filename;

    cmd = new_command_node();
    if (!cmd)
        return (NULL);
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if (is_redirection((*tokens)->type))
        {
            redir_type = (*tokens)->type;
            *tokens = (*tokens)->next;
            filename = (*tokens)->value;
            *tokens = (*tokens)->next;
            cmd = new_redir_node(redir_type, filename, cmd);
        }
        else if ((*tokens)->type == TOKEN_WORD)
        {
            add_arg_to_command(cmd, (*tokens)->value);
            *tokens = (*tokens)->next;
        }
    }
    return cmd;
}

t_ast   *parse_tokens(t_token *tokens)
{
    t_ast   *left;
    t_ast   *right;
    left = NULL;
    right = NULL;
    if (!tokens)
        return NULL;
    left = parse_command_segment(&tokens);

    while (tokens && tokens->type == TOKEN_PIPE)
    {
        tokens = tokens->next;
        right = parse_command_segment(&tokens);
        left = new_pipe_node(left, right);
    }
    return left;
}
