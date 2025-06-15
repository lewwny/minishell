/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:31:09 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:27:55 by lengarci         ###   ########.fr       */
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

char	*get_env_value(const char *key)
{
	t_env	*env;
	char	*value;

	env = _data()->env_list;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			value = ft_strdup(env->value);
			if (!value)
				malloc_error();
			return (value);
		}
		env = env->next;
	}
	return (NULL);
}
