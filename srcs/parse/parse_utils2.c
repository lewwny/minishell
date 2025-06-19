/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:12:27 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/19 13:19:15 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_double_quote_escape(const char *str, unsigned int *j,
			unsigned int *p, t_ctx *buff)
{
	_data()->escaped++;
	if (str[*j + 1] == '$')
		buff->is_escaped = true;
	(*j)++;
	buff->arg[(*p)++] = str[*j];
	(*j)++;
}

void	handle_single_quote_dollar(unsigned int *j, unsigned int *p,
			t_ctx *buff)
{
	_data()->escaped++;
	buff->is_escaped = true;
	buff->arg[(*p)++] = '$';
	(*j)++;
}

t_token	*advance_token(void)
{
	t_data	*minishell;

	minishell = _data();
	return (&minishell->tokens[minishell->pos++]);
}

bool	ensure_str_capacity(char **buf, unsigned int *cap, unsigned int needed)
{
	unsigned int	new_cap;
	char			*new_buf;

	if (needed >= *cap)
	{
		new_cap = *cap * 2;
		new_buf = ft_realloc(*buf, *cap, new_cap);
		if (!new_buf)
			return (false);
		*buf = new_buf;
		*cap = new_cap;
	}
	return (true);
}
