/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:35:50 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:34:59 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	delete_env_node(t_env **env, const char *key)
{
	t_env	*curr;
	t_env	*prev;

	if (!env || !*env || !key)
		return ;
	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			if (curr->value)
				free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	print_env_list(t_env *env)
{
	while (env)
	{
		if (env->key && env->value)
		{
			printf("%s=%s\n", env->key, env->value);
		}
		env = env->next;
	}
}

void	lst_add_back_env(t_env **env, t_env *new_node)
{
	t_env	*curr;

	if (!env || !new_node)
		return ;
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	curr = *env;
	while (curr->next)
		curr = curr->next;
	curr->next = new_node;
}
