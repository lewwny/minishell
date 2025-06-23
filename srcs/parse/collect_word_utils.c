/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_word_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:56:40 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 13:37:26 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ensure_capacity(char **buf, unsigned int *cap, unsigned int len)
{
	if (len + 1 >= *cap)
	{
		*cap *= 2;
		*buf = ft_realloc(*buf, (sizeof(char) * len), (sizeof(char) * (*cap)));
		if (!*buf)
			return (false);
	}
	return (true);
}

static bool	handle_unclosed_quote(t_ctx *ctx, char quote)
{
	(void)quote;
	ctx->unclosed = true;
	_data()->early_error = true;
	return (false);
}

static bool	handle_escaped_char(t_ctx *ctx, const char *str, char quote,
		unsigned int *i)
{
	if (quote == '\"' && str[*i] == '\\'
		&& str[*i + 1] && ft_strchr("\\\"$`", str[*i + 1]))
	{
		if (str[*i + 1] == '$')
			ctx->is_escaped = true;
		(*i)++;
		return (true);
	}
	else if (quote == '\'' && str[*i] == '$')
	{
		ctx->is_escaped = true;
	}
	return (false);
}

static bool	append_char_to_ctx(t_ctx *ctx, char c)
{
	if (!ensure_capacity(&ctx->arg, &ctx->cap, ctx->len))
	{
		ctx->arg = NULL;
		return (false);
	}
	ctx->arg[ctx->len++] = c;
	return (true);
}

bool	collect_quoted_ctx(t_ctx *ctx, const char *str, char quote,
			unsigned int *i)
{
	unsigned int	end;

	ctx->is_escaped = 0;
	ctx->unclosed = false;
	end = find_closing_quote(str + *i, quote);
	if (str[end + *i] != quote)
		return (handle_unclosed_quote(ctx, quote));
	(*i)++;
	while (str[*i] && str[*i] != quote)
	{
		if (handle_escaped_char(ctx, str, quote, i))
			;
		if (!append_char_to_ctx(ctx, str[*i]))
			return (false);
		(*i)++;
	}
	if (str[*i] != quote)
		return (handle_unclosed_quote(ctx, quote));
	(*i)++;
	return (true);
}
