/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:12:47 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/13 12:32:51 by macauchy         ###   ########.fr       */
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
		m->ctx = ft_realloc(m->ctx, (sizeof(t_ctx) * (*c)), \
			sizeof(t_ctx) * (*cap));
		if (!m->args || !m->ctx)
		{
			ft_dprintf(2, "Error: Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
	}
	m->args[*c] = new_tok;
	(*c)++;
}

void	append_t_ctx(unsigned int *cap, unsigned int *c, t_ctx *new_tok)
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
		m->ctx = ft_realloc(m->ctx, (sizeof(t_ctx) * (*c)), \
			sizeof(t_ctx) * (*cap));
		if (!m->args || !m->ctx)
		{
			ft_dprintf(2, "Error: Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
	}
	m->args[*c] = new_tok->arg;
	free(_data()->ctx[*c].arg);
	_data()->ctx[*c].arg = ft_strdup(new_tok->arg);
	_data()->ctx[*c].is_escaped = new_tok->is_escaped;
	(*c)++;
}

bool	handle_operator(unsigned int *cap, unsigned int *count,
							char *line, unsigned int *i)
{
	char	*op_token;
	t_ctx	*ctx;

	op_token = extract_operator(line, i);
	if (!op_token)
	{
		dprintf(2, "Error: Memory allocation failed\n");
		free_split(_data()->args);
	}
	ctx = malloc(sizeof(t_ctx));
	if (!ctx)
	{
		free(op_token);
		dprintf(2, "Error: Memory allocation failed for context\n");
		return (false);
	}
	ctx->arg = op_token;
	append_t_ctx(cap, count, ctx);
	return (true);
}

bool	handle_word(unsigned int *cap, unsigned int *count,
						char *line, unsigned int *i)
{
	char	*token;
	t_ctx	*ctx;

	token = extract_word(line, i);
	ctx = malloc(sizeof(t_ctx));
	ctx->arg = token;
	if (!token)
		return (false);
	append_t_ctx(cap, count, ctx);
	return (true);
}

bool	handle_quote(unsigned int *cap, unsigned int *count,
						char *line, unsigned int *i)
{
	bool			unclosed;
	char			quote;
	t_ctx			*ctx;
	unsigned int	skip_len;

	unclosed = false;
	quote = line[*i];
	_data()->escaped = 0;
	ctx = collect_quoted(line + *i, quote, &unclosed);
	if (unclosed || !ctx)
	{
		dprintf(2, "Error: Unclosed quote '%c'\n", quote);
		_data()->early_error = true;
		free(ctx);
		return (false);
	}
	skip_len = ft_strlen(ctx->arg) + 2;
	append_t_ctx(cap, count, ctx);
	*i += skip_len + _data()->escaped;
	free(ctx);
	return (true);
}
