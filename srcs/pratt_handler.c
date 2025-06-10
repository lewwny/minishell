/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:12:47 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/10 12:17:32 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	append_token(unsigned int *cap, unsigned int *c, char *new_tok)
{
	t_data	*m;

	m = _data();
	if (*c + 1 >= *cap)
	{
		if (*cap == 0)
			*cap = 8;
		else
			*cap *= 2;
		m->args = ft_realloc(m->args, (sizeof(char *) * (*c)), \
			sizeof(char *) * (*cap));
		if (!m->args)
		{
			dprintf(2, "Error: Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
	}
	m->args[*c] = new_tok;
	(*c)++;
}

bool	handle_operator(unsigned int *cap, unsigned int *count,
							char *line, unsigned int *i)
{
	char	*op_token;

	op_token = extract_operator(line, i);
	if (!op_token)
	{
		dprintf(2, "Error: Memory allocation failed\n");
		free_split(_data()->args);
	}
	append_token(cap, count, op_token);
	return (true);
}

bool	handle_word(unsigned int *cap, unsigned int *count,
						char *line, unsigned int *i)
{
	char	*token;

	token = extract_word(line, i);
	if (!token)
		return (false);
	append_token(cap, count, token);
	return (true);
}

bool	handle_quote(unsigned int *cap, unsigned int *count,
						char *line, unsigned int *i)
{
	bool			unclosed;
	char			quote;
	char			*qstr;
	unsigned int	skip_len;

	unclosed = false;
	quote = line[*i];
	qstr = collect_quoted(line + *i, quote, &unclosed);
	if (unclosed || !qstr)
	{
		dprintf(2, "Error: Unclosed quote '%c'\n", quote);
		_data()->early_error = true;
		return (false);
	}
	skip_len = ft_strlen(qstr) + 2;
	append_token(cap, count, qstr);
	*i += skip_len + _data()->escaped;
	return (true);
}
