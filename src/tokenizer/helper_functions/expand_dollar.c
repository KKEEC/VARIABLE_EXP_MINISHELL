#include <stdio.h>
#include <stdlib.h>
#include "../../../includes/minishell.h"
#include "../../../includes/tokenizer.h"
#include "../../../includes/env.h"
#include <string.h>
#include <unistd.h>
 #include <stdlib.h>

static int  num_len(int n)
{
    int len = 0;
    if (n <= 0)
        len++; // for '-' or '0'
    while (n)
    {
        n /= 10;
        len++;
    }
    return (len);
}

char *ft_itoa(int n)
{
    int len = num_len(n);
    char *str = malloc(len + 1);
    unsigned int num;
    
    if (!str)
        return NULL;
    str[len] = '\0';
    if (n < 0)
    {
        str[0] = '-';
        num = -n;
    }
    else
        num = n;
    if (num == 0)
        str[0] = '0';
    while (num)
    {
        str[--len] = (num % 10) + '0';
        num /= 10;
    }
    return str;
}

char	*append_pid(char *result)
{
	char	*pid_str;
	char	*tmp;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (ft_strdup(result));
	tmp = ft_strjoin(result, pid_str);
	free(pid_str);
	return (tmp);
}

char	*append_char(char *result, char c)
{
	char	buff[2];
	char	*tmp;

	buff[0] = c;
	buff[1] = '\0';
	tmp = ft_strjoin(result, buff);
	return (tmp);
}

char	*append_var(char *result, char **tstr, t_env *env_list)
{
	char		*start;
	char	*key;
	char	*value;
	char	*tmp;

	start = *tstr;
	while (**tstr && (ft_isalnum(**tstr) || **tstr == '_'))
		(*tstr)++;
	key = ft_strndup(start, *tstr - start);
	//printf("Key: %s\n", key);
	value = get_env_value(env_list, key);
	free(key);
	if (!value)
	{
		tmp = ft_strdup(result);
		return (tmp);
	}
	tmp = ft_strjoin(result, value);
	return (tmp);
}

char	*handle_dollar(char *result, char **tstr, int *status, t_env *env_list)
{
	(*tstr)++;
	//printf("Handling dollar: %s\n", *tstr);
	if (!**tstr)
		return (append_char(result, '$'));
	if (**tstr == '$')
	{
		(*tstr)++;
		return (append_pid(result));
	}
    else if (**tstr == '?')
    {
        (*tstr)++;
        char *status_str = ft_itoa(*status);
        if (!status_str)
            return (ft_strdup(result));
        char *tmp = ft_strjoin(result, status_str);
        free(status_str);
        return tmp;
    }
	else if (ft_isalpha(**tstr) || **tstr == '_')
		return (append_var(result, tstr, env_list));
	return (append_char(result, '$'));
}

char	*expanddollar(const char *str, t_env *env_list, int *status)
{
	char	*result;;
	char *temp;
	char *tstr;

	tstr = (char *)str;
	result = ft_strdup("");
	//printf("Expanding dollar in: %s %s\n", tstr, str);
	while (*tstr)
	{
		if (*tstr == '$')
		{
			temp = handle_dollar(result, &tstr, status, env_list);
			free(result);
			result = temp;
		}
		else
		{
			temp = append_char(result, *tstr);
			free(result);
			result = temp;
			tstr++;
		}
	}
	return (result);
}