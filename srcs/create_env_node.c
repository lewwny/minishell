/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:26:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/12 15:32:13 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	assign_env_key_value(t_env *new_node, char **kv)
{
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
}

static t_env	*alloc_env_node(char **kv)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		malloc_error();
	assign_env_key_value(new_node, kv);
	new_node->next = NULL;
	return (new_node);
}

t_env	*create_env_node(char *env)
{
	t_env	*new_node;
	char	**kv;

	kv = ft_split(env, '=');
	if (!kv || !kv[0])
	{
		free_split(kv);
		return (NULL);
	}
	new_node = alloc_env_node(kv);
	free_split(kv);
	return (new_node);
}
