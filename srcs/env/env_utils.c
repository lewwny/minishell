/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:07:23 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:33:32 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	in_env(const char *key)
{
	t_env	*env;

	env = _data()->env_list;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

static void	edit_value(const char *key, const char *value)
{
	t_env	*env;

	env = _data()->env_list;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			if (!env->value)
				malloc_error();
			return ;
		}
		env = env->next;
	}
}

static void	split_env(const char *env, char **key, char **value)
{
	char	**kv;

	*key = NULL;
	*value = NULL;
	kv = ft_split(env, '=');
	if (!kv || !kv[0])
	{
		free_split(kv);
		return ;
	}
	*key = ft_strdup(kv[0]);
	if (kv[1])
		*value = ft_strdup(kv[1]);
	free_split(kv);
}

static t_env	*create_enve_node(const char *env)
{
	t_env	*new_node;
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	split_env(env, &key, &value);
	if (!key)
		return (NULL);
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		malloc_error();
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

void	edit_env(t_env *env_list)
{
	char	*pwd;
	t_env	*new_node;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		malloc_error();
	if (in_env("PWD"))
	{
		edit_value("PWD", pwd);
		free(pwd);
	}
	else
	{
		new_node = create_enve_node("PWD=");
		if (!new_node)
			malloc_error();
		new_node->value = pwd;
		new_node->next = env_list;
		_data()->env_list = new_node;
	}
}
