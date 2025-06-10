/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:49:14 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/10 16:26:04 by lengarci         ###   ########.fr       */
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
	if (!new_node->key)
	{
		free(new_node);
		free_split(kv);
		malloc_error();
	}
	if (kv[1])
	{
		new_node->value = ft_strdup(kv[1]);
		if (!new_node->value)
		{
			free(new_node->key);
			free(new_node);
			free_split(kv);
			malloc_error();
		}
	}
	else
		new_node->value = NULL;
	new_node->next = NULL;
	free_split(kv);
	return (new_node);
}

t_env	*env_fill(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	int		i;

	env_list = NULL;
	i = 0;
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

static int	env_list_size_with_value(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		if (env->key && env->value)
			size++;
		env = env->next;
	}
	return (size);
}

static char	*env_entry_to_str(t_env *env)
{
	char	*entry;

	entry = malloc(ft_strlen(env->key) + ft_strlen(env->value) + 2);
	if (!entry)
		malloc_error();
	ft_strcpy(entry, env->key);
	ft_strcat(entry, "=");
	ft_strcat(entry, env->value);
	return (entry);
}

static char	*free_array(char **res, int j, char *tmp)
{
	int	i;

	if (!tmp)
	{
		i = 0;
		while (i < j)
			free(res[i++]);
		free(res);
		return (NULL);
	}
	return (tmp);
}

char	**env_to_array(t_env *env)
{
	char	**array;
	char	*entry;
	int		size;
	int		i;

	i = 0;
	size = env_list_size_with_value(env);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		malloc_error();
	while (env)
	{
		if (env->key && env->value)
		{
			entry = env_entry_to_str(env);
			if (!free_array(array, i, entry))
				malloc_error();
			array[i++] = entry;
		}
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}
