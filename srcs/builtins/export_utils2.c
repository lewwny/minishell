/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:21:48 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/19 11:28:40 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_key(char *arg)
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

int	in_env(char *key)
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

void	edit_value(char *key, char *value)
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
