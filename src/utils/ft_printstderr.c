#include <unistd.h>

void ft_printstderr(const char *str)
{
    if (!str)
        return;
    size_t len = 0;
    while (str[len])
        len++;
    write(2, str, len);
}
