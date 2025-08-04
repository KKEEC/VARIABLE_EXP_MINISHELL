#include "../../includes/env.h"

// char *safe_malloc(size_t size)
// {
// 	char *new_node = malloc(size);
// 	if (!new_node)
// 		return NULL;
// 	return new_node;
// }

inline
static void handle_empty_env(t_env **env_list, const char *key, const char *value)
{
	t_env *new_node;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return;
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	new_node->next = NULL;
	*env_list = new_node;
}

inline
static int handle_key_exist(t_env *current, const char *key, const char *value)
{
	if (isstrequal(current->key, key))
	{
		free(current->value);
		if (value)
			current->value = ft_strdup(value);
		return 1;
	}
	return 0;
}
inline
static void handle_new_env(t_env *new_node, const char *key, const char *value)
{
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
}
void    update_env(t_env **env_list, const char *key, const char *value)
{
	t_env   *current;
	t_env   *new_node;

	if (!key)
		return ;
	if (!*env_list)
	{
		handle_empty_env(env_list, key, value);
		return ;
	}
	current = *env_list;
	while (current)
	{
		if (handle_key_exist(current, key, value))
			return ;
		if (!current->next)
		    break ;
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	return ;
	handle_new_env(new_node, key, value);
	new_node->next = NULL;
	current->next = new_node;
}
