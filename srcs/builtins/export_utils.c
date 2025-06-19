/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:31:09 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/19 11:27:07 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static int	env_list_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static void	sort_env_array(t_env **arr, int count)
{
	int		i;
	int		swapped;
	t_env	*tmp;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < count - 1)
		{
			if (ft_strcmp(arr[i]->key, arr[i + 1]->key) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}

static t_env	**enve_to_array(t_env *env, int *count)
{
	t_env	**arr;
	t_env	*tmp;
	int		i;

	*count = env_list_size(env);
	arr = malloc(sizeof(t_env *) * (*count));
	if (!arr)
		malloc_error();
	tmp = env;
	i = 0;
	while (tmp)
	{
		arr[i++] = tmp;
		tmp = tmp->next;
	}
	sort_env_array(arr, *count);
	return (arr);
}

void	print_export(t_env *env)
{
	int		count;
	int		i;
	t_env	**arr;

	arr = enve_to_array(env, &count);
	i = 0;
	while (i < count)
	{
		if (arr[i]->value)
			printf("declare -x %s=\"%s\"\n", arr[i]->key, arr[i]->value);
		else
			printf("declare -x %s\n", arr[i]->key);
		i++;
	}
	free(arr);
}
