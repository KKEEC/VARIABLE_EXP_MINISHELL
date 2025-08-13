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

char	*append_var(char *result, const char *str, int *i, t_env *env_list)
{
	int		start;
	char	*key;
	char	*value;
	char	*tmp;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	key = ft_strndup(&str[start], *i - start);
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

char	*handle_dollar(char *result, const char *str, int *i, t_env *env_list)
{
	(*i)++;
	if (!str[*i])
		return (append_char(result, '$'));
	if (str[*i] == '$')
	{
		(*i)++;
		return (append_pid(result));
	}
    else if (str[*i] == '?')
    {
        (*i)++;
        char *status_str = ft_itoa(get_shell_status());
        if (!status_str)
            return (ft_strdup(result));
        char *tmp = ft_strjoin(result, status_str);
        free(status_str);
        return tmp;
    }
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
		return (append_var(result, str, i, env_list));
	return (append_char(result, '$'));
}

char	*expanddollar(const char *str, t_env *env_list)
{
	char	*result;
	int		i;
	char *temp;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			temp = handle_dollar(result, str, &i, env_list);
			free(result);
			result = temp;
		}
		else
		{
			temp = append_char(result, str[i++]);
			free(result);
			result = temp;
		}
	}
	return (result);
}