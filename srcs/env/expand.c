/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:01:17 by lenygarcia        #+#    #+#             */
/*   Updated: 2025/06/22 11:48:54 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*extract_env_key(const char *key_start, char **key_end)
{
	*key_end = (char *)(key_start + 1);
	while (ft_isalnum(**key_end) || **key_end == '_')
		(*key_end)++;
	if (*key_end == key_start + 1)
		return (NULL);
	return (ft_substr(key_start + 1, 0, *key_end - (key_start + 1)));
}

static char	*handle_special_or_invalid_key(char *result, char **str,
			char *key_start, char *key)
{
	if (key_start[1] == '?')
	{
		free(key);
		result = ft_strjoin_free(result, ft_itoa(_data()->exit_code), 1, 1);
		if (!result)
			malloc_error();
		*str = key_start + 2;
		return (result);
	}
	else if (!key)
	{
		result = ft_strjoin_free(result, ft_strdup("$"), 1, 1);
		if (!result)
			malloc_error();
		*str = key_start + 1;
		return (result);
	}
	return (NULL);
}

static char	*process_env_var_segment(char *result, char **str, char *key_start)
{
	char	*env_value;
	char	*key_end;
	char	*key;
	char	*special_result;

	key = extract_env_key(key_start, &key_end);
	special_result = handle_special_or_invalid_key(result, str, key_start, key);
	if (special_result)
		return (special_result);
	env_value = get_env_value(key);
	free(key);
	if (env_value)
	{
		result = ft_strjoin_free(result, env_value, 1, 1);
		if (!result)
			malloc_error();
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
		if (!result)
			malloc_error();
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
	if (!result)
		malloc_error();
	return (result);
}
