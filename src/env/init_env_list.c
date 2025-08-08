#include "../../includes/env.h"

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

void	add_env_node(t_env **env_list, t_env *new_node)
{
	t_env	*temp;

	if (!*env_list)
		*env_list = new_node;
	else
	{
		temp = *env_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

t_env	*init_env_list(char **envp, t_env **env_list)
{
	t_env	*new_node;
	int		i;
	char	*equal;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (!equal)
		{
			i++;
			continue ;
		}
		key = ft_strndup(envp[i], (equal - envp[i]));
		value = ft_strdup(equal + 1);
		new_node = new_env_node(key, value);
		free(key);
		free(value);
		add_env_node(env_list, new_node);
		i++;
	}
	return (*env_list);
}
