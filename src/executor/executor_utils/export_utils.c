#include "../../../includes/executor.h"

void	print_err_key(char *key)
{
	ft_printstderr("export: `");
	ft_printstderr(key);
	ft_printstderr("`: not a valid identifier\n");
}
