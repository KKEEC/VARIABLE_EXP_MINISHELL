#include "../../../includes/executor.h"

int builtin_pwd(void)
{
    char    *currentdir;

    currentdir = getcwd(NULL, 0);

    if (!currentdir)
    {
        perror("pwd");
        return (1);  
    }
    printf("%s\n", currentdir);
    free(currentdir);
    return (0);
}