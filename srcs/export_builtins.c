/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:20:01 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/10 15:43:22 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_key(char *arg)
{
	char	*eq;
	char	*tmp;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		tmp = ft_strdup(arg);
		if (!tmp)
			malloc_error();
		return (tmp);
	}
	return (ft_substr(arg, 0, eq - arg));
}

static int	in_env(char *key)
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

static void	edit_value(char *key, char *value)
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

static void	export_update_env(char *key, char *value)
{
	t_env	*new_node;

	if (in_env(key))
	{
		if (value)
			edit_value(key, value);
	}
	else
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			malloc_error();
		new_node->key = ft_strdup(key);
		if (!new_node->key)
		{
			free(new_node);
			malloc_error();
		}
		if (value)
		{
			new_node->value = ft_strdup(value);
			if (!new_node->value)
			{
				free(new_node->key);
				free(new_node);
				malloc_error();
			}
		}
		else
			new_node->value = NULL;
		new_node->next = NULL;
		lst_add_back_env(&_data()->env_list, new_node);
	}
}

void	export_builtin(void)
{
	char	*key;
	char	*value;
	char	*eq;

	if (_data()->cmds->args[1] == NULL)
	{
		print_env_list(_data()->env_list);
		return ;
	}
	if (_data()->cmds->args[1][0] == '\0' || _data()->cmds->args[1][0] == '=')
		return ;
	eq = ft_strchr(_data()->cmds->args[1], '=');
	key = get_key(_data()->cmds->args[1]);
	if (eq)
		value = eq + 1;
	else
		value = NULL;
	export_update_env(key, value);
	free(key);
}
