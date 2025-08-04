#include "../../../includes/tokenizer.h"

void    free_tokens(t_token *head)
{
    t_token *tmp;

    while(head)
    {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}

void    add_token(t_token **head, t_token *new_token)
{
    t_token *cur;

    if (!*head)
        *head = new_token;
    else
    {
        cur = *head;
        while (cur->next)
            cur = cur->next;
        cur->next = new_token; 
    }
}

t_token *create_token(t_token_type type, const char *input)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = ft_strdup(input);
    token->next = NULL;
    return token;
}
