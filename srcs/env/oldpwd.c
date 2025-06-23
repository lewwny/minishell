/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oldpwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:09:56 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/20 15:34:53 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	change_oldpwd(char *oldpwd)
{
	t_env	*env;
	t_env	*new_node;

	env = _data()->env_list;
	while (env)
	{
		if (ft_strcmp(env->key, "OLDPWD") == 0)
		{
			free(env->value);
			env->value = ft_strdup(oldpwd);
			if (!env->value)
				malloc_error();
			return ;
		}
		env = env->next;
	}
	if (!env)
	{
		new_node = create_env_node("OLDPWD");
		new_node->value = ft_strdup(oldpwd);
		if (!new_node->value)
			malloc_error();
		new_node->next = _data()->env_list;
		_data()->env_list = new_node;
	}
}
