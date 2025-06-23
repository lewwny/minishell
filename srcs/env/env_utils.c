/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:07:23 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 11:35:36 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	in_enve(const char *key)
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

static void	edite_value(const char *key, const char *value)
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

void	update_pwd_value(const char *pwd)
{
	t_env	*new_node;

	if (in_enve("PWD"))
	{
		edite_value("PWD", pwd);
		free((char *)pwd);
	}
	else
	{
		new_node = create_enve_node("PWD=");
		if (!new_node)
			malloc_error();
		new_node->value = (char *)pwd;
		new_node->next = _data()->env_list;
		_data()->env_list = new_node;
	}
}

void	edit_env(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		if (errno == ENOENT)
		{
			write(2, "minishell: pwd: No such file or directory\n", 43);
			pwd = ft_strdup("/");
			if (!pwd)
				malloc_error();
		}
		else
			malloc_error();
	}
	update_pwd_value(pwd);
}
