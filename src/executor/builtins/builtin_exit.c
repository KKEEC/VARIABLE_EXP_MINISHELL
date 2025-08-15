#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "../../../includes/utils.h"

static int	is_number(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	builtin_exit(char **args)
{
	int	exit_code;

	exit_code = 0;
	if (!args[1])
		return (128 + exit_code);
	if (!is_number(args[1]))
	{
		ft_printstderr("bash: exit: ");
		ft_printstderr(args[1]);
		ft_printstderr(": numeric argument required\n");
		return (128 + 2);
	}
	if (args[2])
	{
		ft_printstderr("bash: exit: too many arguments\n");
		return (1);
	}
	exit_code = ft_atoi(args[1]);
	exit_code = ((exit_code % 256) + 256) % 256;
	return (128 + exit_code);
}
