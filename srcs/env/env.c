/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:49:14 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:33:37 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
