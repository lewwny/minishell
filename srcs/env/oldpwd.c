/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oldpwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:09:56 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:51:56 by lengarci         ###   ########.fr       */
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
			return ;
		}
		env = env->next;
	}
	if (!env)
	{
		new_node = create_env_node("OLDPWD");
		new_node->value = ft_strdup(oldpwd);
		new_node->next = _data()->env_list;
		_data()->env_list = new_node;
	}
}
