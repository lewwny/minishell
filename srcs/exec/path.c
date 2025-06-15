/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:24:29 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:34:02 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	put_slash(char **path)
{
	int	i;
	int	len;

	i = 0;
	if (!path || !*path)
		return ;
	while (path[i])
	{
		len = ft_strlen(path[i]);
		path[i][len] = '/';
		path[i][len + 1] = '\0';
		i++;
	}
}

static char	*get_enve_value(t_env *env, const char *key)
{
	while (env)
	{
		if (env->key && ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	get_path(void)
{
	t_data	*data;
	char	*path_value;

	data = _data();
	path_value = get_enve_value(data->env_list, "PATH");
	if (path_value)
	{
		data->path = ft_split(path_value, ':');
		if (!data->path)
			malloc_error();
	}
	else
		data->path = NULL;
	put_slash(data->path);
}
