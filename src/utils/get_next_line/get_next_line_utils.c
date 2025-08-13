#include "get_next_line.h"

char	*gnl_ft_strchr(const char *str, int c)
{
	if (!str)
		return (NULL);
	while (*str)
	{
		if (*str == (char)c)
			return ((char *)str);
		str++;
	}
	if (c == '\0')
		return ((char *)str);
	return (NULL);
}

char	*gnl_ft_strdup(const char *str)
{
	int		i;
	char	*dup;

	i = 0;
	dup = (char *)malloc(gnl_ft_strlen(str) * sizeof(char) + 1);
	if (!dup)
		return (NULL);
	while (str[i] != '\0')
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static	char	*ft_copynjoin(const char *s1, const char *s2, char *rstr)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		rstr[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		rstr[i + j] = s2[j];
		j++;
	}
	rstr[i + j] = '\0';
	return (rstr);
}

char	*gnl_ft_strjoin(const char *s1, const char *s2)
{
	int		s1len;
	int		s2len;
	char	*rstr;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (gnl_ft_strdup(s2));
	if (!s2)
		return (gnl_ft_strdup(s1));
	s1len = gnl_ft_strlen(s1);
	s2len = gnl_ft_strlen(s2);
	rstr = (char *)malloc((s1len + s2len) * sizeof(char) + 1);
	if (!rstr)
		return (NULL);
	return (ft_copynjoin(s1, s2, rstr));
}

size_t	gnl_ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (!src)
		return (0);
	if (dstsize > 0)
	{
		while (i < dstsize - 1 && src[i] != '\0')
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (gnl_ft_strlen(src));
}