#include "../../../includes/parser.h"

void    free_ast(t_ast *node)
{
    int i;

    i = 0;
    if (!node)
        return ;
    free_ast(node->left);
    free_ast(node->right);

    if (node->type == NODE_COMMAND && node->args)
    {
        while (node->args[i])
        {
            free(node->args[i]);
            i++;
        }
        free(node->args);
    }
    if (node->type == NODE_REDIR && node->filename)
        free(node->filename);
    free(node);
    node = NULL;
}