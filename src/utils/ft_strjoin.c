#include <stdlib.h>
#include "../../includes/utils.h"

char    *ft_strjoin(char const *s1, char const *s2)
{
    char	*nstr;
    size_t	i;
    size_t	j;

    if (!s1 || !s2)
        return (NULL);
    nstr = (char *)malloc((ft_strlen(s1) + ft_strlen(s2)) + 1);
    if (!nstr)
        return (NULL);
    i = 0;
    while (s1[i])
    {
        nstr[i] = s1[i];
        i++;
    }
    j = 0;
    while (s2[j])
    {
        nstr[i + j] = s2[j];
        j++;
    }
    nstr[i + j] = '\0';
    return (nstr);
}