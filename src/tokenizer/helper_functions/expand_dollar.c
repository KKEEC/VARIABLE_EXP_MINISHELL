#include <stdio.h>
#include <stdlib.h>
#include "../../../includes/tokenizer.h"
#include "../../../includes/env.h"
#include <string.h>

static char *slice_key(int i, const char *str)
{
    int j = i;
    char *res;
    int len;
    len = 0;
    while (str[j]) 
    {
        printf("%c\n", str[j]);
        if ((str[j] >= 9 && str[j] <= 13) || str[j] == 32)
            break ;
        else
            len++;
        j++;
    }
    printf("index : %d len: %d \n", j, len);
    res = malloc(len + 1);
    j = 0;
    int k = i;
    while(str[k])
    {
        if ((str[k] >= 9 && str[k] <= 13) || str[k] == 32)
            break ;
        else 
            res[j] = str[k];
        j++;
        k++;
    }
    res[j] = '\0';
    printf("RESULT: KEY : %ld\n", strlen(res));
    return res;
}

char	*expanddollar(const char *str, t_env *env_list)
{
	char *res;
	int	i;
	char *f_half;

    i = 0;
    printf("string: %s\n", str);
	while(str[i] && str[i] != '$')
		i++;
	f_half = malloc(i + 1);
    i++;
    char *key = slice_key(i, str);
    printf("PRINT KEY: %s\n", key);
	i = 0;
	while (str[i] && str[i] != '$')
	{
		f_half[i] = str[i];
		i++;
	}
    f_half[i] = '\0';
    printf("PRINT KEY: %s\n first_half: %s\n", key, f_half);
	if (!get_env_value(env_list, key))
		return (NULL);
    res = ft_strjoin(f_half, get_env_value(env_list, key));
	printf("result: %s\n", res);
	return res;
}
