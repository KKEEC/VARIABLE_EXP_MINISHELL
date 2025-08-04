#include "../../includes/utils.h"

int ft_isspace(const char c)
{
    if ((c >= 9 && c <= 13) || c == 32)
        return (1);
    return (0);
}