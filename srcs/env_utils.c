/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:07:23 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/10 15:07:38 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
