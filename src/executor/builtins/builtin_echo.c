#include "../../../includes/executor.h"

static int  is_n_flag(const char *arg)
{
    int i;

    i = 1;
    if (!arg || arg[0] != '-')
        return (0);
    if (arg[1] == '\0')
        return (0);
    while (arg[i])
    {
        if (arg[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

int builtin_echo(char **args)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    if (!args)
        return (1);
    while (args[i] && is_n_flag(args[i]))
    {
        newline = 0;
        i++;
    }
    //print args
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}
