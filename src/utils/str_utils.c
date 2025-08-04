#include "../../includes/tokenizer.h"

size_t  ft_strlen(const char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return i;
}

int isstrequal(const char *s1, const char *s2)
{
    int i;

    i = 0;
    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return (0);
        i++;
    }
    if (s1[i] == '\0' && s2[i] == '\0')
        return (1);
    return (0);

}

int ft_isalpha(const char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return 1;
    else
        return 0;
}

int ft_isalnum(const char c)
{
    if ((c >= '0' && c <= '9') || ft_isalpha(c))
        return (1);
    else
        return (0);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*sstr;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	sstr = (char *)malloc(sizeof(char) * (len + 1));
	if (!sstr)
		return (NULL);
	while (s[i])
	{
		if (i >= start && j < len)
		{
			sstr[j] = s[i];
			j++;
		}
		i++;
	}
	sstr[j] = '\0';
	return (sstr);
}

char	*ft_strdup(const char *str)
{
	size_t	len;
	char	*dup;
    size_t  i;

    i = 0;
	len = ft_strlen(str);
	dup = malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	while (i < len)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}


char *ft_strndup(const char *str, size_t n)
{
    size_t len;
    size_t i;
    char *dup;
    
    len = 0;
    i = 0;
    while (str[len] && len < n)
        len++;
    dup = malloc(len + 1);
    if (!dup)
        return NULL;
    while(i < len)
    {
        dup[i] = str[i];
        i++;
    }
    dup[len] = '\0';
    return dup;
}