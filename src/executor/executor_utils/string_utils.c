/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkeec <kkeec@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 00:00:00 by kkeec             #+#    #+#             */
/*   Updated: 2025/08/18 00:00:00 by kkeec            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/executor.h"

static char	*create_path_segment(char *path, int i, int j)
{
	int		k;
	int		len;
	char	*str1;

	k = 0;
	len = j - i;
	str1 = malloc(len + 1);
	if (!str1)
		return (NULL);
	while (i < j)
	{
		str1[k] = path[i];
		k++;
		i++;
	}
	str1[k] = '\0';
	return (str1);
}

char	*ft_strncpyandjoin(char *path, int i, int j, char *cmd)
{
	char	*str1;
	char	*strslash;
	char	*res;

	str1 = create_path_segment(path, i, j);
	if (!str1)
		return (NULL);
	strslash = ft_strjoin(str1, "/");
	if (!strslash)
	{
		free(str1);
		return (NULL);
	}
	res = ft_strjoin(strslash, cmd);
	if (!res)
	{
		free(strslash);
		free(str1);
		return (NULL);
	}
	free(str1);
	free(strslash);
	return (res);
}

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_strcpy(result, s1);
	ft_strcpy(result + len1, s2);
	ft_strcpy(result + len1 + len2, s3);
	result[len1 + len2 + len3] = '\0';
	return (result);
}
