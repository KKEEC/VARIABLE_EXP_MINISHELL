#include "../../includes/utils.h"

int ft_isschar(const char c)
{
    if (c == '|' || c == '<' || c == '>')
        return (1);
    return (0);
}