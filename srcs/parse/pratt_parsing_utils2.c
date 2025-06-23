/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parsing_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:09:45 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 15:21:39 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	handle_quoted_inner(const char *line, unsigned int *i, t_ctx *frag,
		char quote)
{
	while (line[*i] && line[*i] != quote)
	{
		if (quote == '"' && line[*i] == '\\'
			&& ft_strchr("\"\\$`", line[*i + 1]))
		{
			if (line[*i + 1] == '$')
				frag->is_escaped = true;
			(*i)++;
		}
		else if (quote == '\'' && line[*i] == '$')
			frag->is_escaped = true;
		if (!ensure_str_capacity(&frag->arg, &frag->cap, frag->len + 2))
			return (false);
		frag->arg[frag->len++] = line[(*i)++];
	}
	return (true);
}

bool	handle_quoted(const char *line, unsigned int *i, t_ctx *frag)
{
	char	quote;

	quote = line[(*i)++];
	if (!handle_quoted_inner(line, i, frag, quote))
		return (false);
	if (line[*i] != quote)
	{
		frag->unclosed = true;
		return (false);
	}
	(*i)++;
	return (true);
}

bool	handle_unquoted(const char *line, unsigned int *i, t_ctx *frag)
{
	while (line[*i] && !ft_isspace(line[*i]) && !ft_strchr("|<>\"\'", line[*i]))
	{
		if (!ensure_str_capacity(&frag->arg, &frag->cap, frag->len + 2))
			return (false);
		frag->arg[frag->len++] = line[(*i)++];
	}
	return (true);
}

bool	add_fragment(t_ctx **fragments, size_t *idx, size_t *cap,
		t_ctx *frag)
{
	t_ctx	*new;

	if (*idx >= *cap)
	{
		*cap *= 2;
		new = realloc(*fragments, sizeof(t_ctx) * (*cap));
		if (!new)
		{
			free(frag->arg);
			free(*fragments);
			return (false);
		}
		*fragments = new;
	}
	(*fragments)[(*idx)++] = *frag;
	return (true);
}

bool	collect_one_fragment(const char *line, unsigned int *i, t_ctx *frag)
{
	if (!init_fragment(frag))
	{
		free(frag->arg);
		return (false);
	}
	if (line[*i] == '\'' || line[*i] == '"')
	{
		if (!handle_quoted(line, i, frag))
		{
			free(frag->arg);
			frag->arg = NULL;
			return (false);
		}
	}
	else
	{
		if (!handle_unquoted(line, i, frag))
		{
			free(frag->arg);
			frag->arg = NULL;
			return (false);
		}
	}
	frag->arg[frag->len] = '\0';
	return (true);
}
