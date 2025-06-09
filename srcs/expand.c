/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:01:17 by lenygarcia        #+#    #+#             */
/*   Updated: 2025/06/09 19:33:42 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_env_value(const char *key)
{
	t_env	*env;

	env = _data()->env_list;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

static char	*process_env_var_segment(char *result, char **str, char *key_start)
{
	char	*env_value;
	char	*key_end;
	char	*temp;

	key_end = key_start + 1;
	while (ft_isalnum(*key_end) || *key_end == '_')
		key_end++;
	if (key_end == key_start + 1)
		result = ft_strjoin_free(result, ft_strdup("$"), 1, 1);
	else
	{
		temp = ft_substr(key_start + 1, 0, key_end - (key_start + 1));
		env_value = get_env_value(temp);
		free(temp);
		if (env_value)
			result = ft_strjoin_free(result, env_value, 1, 1);
	}
	*str = key_end;
	return (result);
}

static char	*process_env_var(char *result, char **str)
{
	char	*key_start;

	key_start = ft_strchr(*str, '$');
	while (key_start)
	{
		result = ft_strjoin_free(result, ft_substr(*str, 0, key_start - *str),
				1, 1);
		result = process_env_var_segment(result, str, key_start);
		key_start = ft_strchr(*str, '$');
	}
	return (result);
}

char	*replace_env_vars(char *str)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		malloc_error();
	result = process_env_var(result, &str);
	result = ft_strjoin_free(result, str, 1, 0);
	return (result);
}
