#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <stdint.h>
# include <readline/readline.h>
# include <readline/history.h>
#include "env.h"

extern int g_status;

void    minishell_loop(t_env *env_list, int *status);
void    set_shell_status(int status);
int     get_shell_status(void);

#endif