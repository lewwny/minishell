/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:49:14 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/03 17:10:07 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env	*create_env_node(char *env)
{
	t_env	*new_node;
	char	**kv;

	kv = ft_split(env, '=');
	if (!kv || !kv[0])
	{
		free_split(kv);
		return (NULL);
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		malloc_error();
	new_node->key = ft_strdup(kv[0]);
	if (kv[1])
		new_node->value = ft_strdup(kv[1]);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	free_split(kv);
	return (new_node);
}

t_env	*env_fill(char **envp)
{
	t_env	*env_list = NULL;
	t_env	*new_node;
	int		i = 0;

	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (new_node)
		{
			new_node->next = env_list;
			env_list = new_node;
		}
		i++;
	}
	return (env_list);
}

void	delete_env_node(t_env **env, const char *key)
{
	t_env	*curr;
	t_env	*prev;

	if (!env || !*env || !key)
		return ;
	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			if (curr->value)
				free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
static int	env_list_size_with_value(t_env *env)
{
	int size = 0;
	while (env)
	{
		if (env->key && env->value)
			size++;
		env = env->next;
	}
	return size;
}

char	**env_to_array(t_env *env)
{
	char	**array;
	char	*entry;
	int		size;
	int		i = 0;

	size = env_list_size_with_value(env);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	while (env)
	{
		if (env->key && env->value)
		{
			entry = malloc(ft_strlen(env->key) + ft_strlen(env->value) + 2);
			if (!entry)
				return (NULL);
			ft_strcpy(entry, env->key);
			ft_strcat(entry, "=");
			ft_strcat(entry, env->value);
			array[i++] = entry;
		}
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}
