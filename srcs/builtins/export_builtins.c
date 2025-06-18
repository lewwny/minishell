/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:20:01 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/18 14:31:13 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static t_env	*create_new_env_node(char *key, char *value)
{
	t_env	*new_node;

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
	return (new_node);
}

static void	add_or_update_env(char *key, char *value)
{
	t_env	*new_node;

	if (in_env(key))
	{
		if (value)
			edit_value(key, value);
	}
	else
	{
		new_node = create_new_env_node(key, value);
		lst_add_back_env(&_data()->env_list, new_node);
	}
}

static bool	verify_key(t_cmd *cmd)
{
	int	i;
	
	i = 0;
	while (cmd->args[i])
	{
		if (!ft_isalpha(cmd->args[i][0]) && cmd->args[i][0] != '_')
		{
			dprintf(2, "minishell: export: `%s': not a valid identifier\n", cmd->args[i]);
			_data()->exit_code = 1;
			return (false);
		}
		i++;
	}
	return (true);
}

void	export_builtin(void)
{
	char	*key;
	char	*value;
	char	*eq;
	int		i;

	i = 1;
	if (_data()->cmds->args[1] == NULL)
	{
		print_export(_data()->env_list);
		return ;
	}
	if (!verify_key(_data()->cmds))
		return ;
	while (_data()->cmds->args[i])
	{
		if (_data()->cmds->args[i][0] == '\0' ||
			_data()->cmds->args[i][0] == '=')
			return ;
		eq = ft_strchr(_data()->cmds->args[i], '=');
		key = get_key(_data()->cmds->args[i]);
		if (eq)
			value = eq + 1;
		else
			value = NULL;
		add_or_update_env(key, value);
		free(key);
		i++;
	}
}
