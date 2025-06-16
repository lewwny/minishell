/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecauchy <mecauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:41:14 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/16 22:02:48 by mecauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool ensure_capacity(char **buf, unsigned int *cap, unsigned int len)
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

static bool	collect_quoted_ctx(t_ctx *ctx, const char *str, char quote,
		unsigned int *i)
{
	unsigned int	end;

	ctx->is_escaped = 0;
	ctx->unclosed = false;
	end = find_closing_quote(str + *i, quote);
	if (str[end + *i] != quote)
	{
		ctx->unclosed = true;
		_data()->early_error = true;
		return (false);
	}
	(*i)++;
	while (str[*i] && str[*i] != quote)
	{
		if (quote == '\"' && str[*i] == '\\'
			&& str[*i + 1] && ft_strchr("\\\"$`", str[*i + 1]))
		{
			if (str[*i + 1] == '$')
				ctx->is_escaped = true;
			(*i)++;
		}
		else if (quote == '\'' && str[*i] == '$')
			ctx->is_escaped = true;
		if (!ensure_capacity(&ctx->arg, &ctx->cap, ctx->len))
		{
			ctx->arg = NULL;
			return (false);
		}
		ctx->arg[ctx->len++] = str[(*i)++];
	}
	if (str[*i] != quote)
	{
		ctx->unclosed = true;
		_data()->early_error = true;	
		return (false);
	}
	(*i)++;
	return (true);
}

static bool	collect_plain_ctx(t_ctx *ctx, const char *str, unsigned int *i)
{
	while (str[*i] && !ft_isspace(str[*i])
		&& !ft_strchr("|<>\'\"", str[*i]))
	{
		if (!ensure_capacity(&ctx->arg, &ctx->cap, ctx->len))
		{
			ctx->arg = NULL;
			return (false);
		}
		ctx->arg[ctx->len++] = str[(*i)++];
	}
	return (true);
}

t_ctx	collect_word_ctx(const char *line, unsigned int *i)
{
	t_ctx			ctx;

	ft_bzero(&ctx, sizeof(ctx));
	ctx.cap = 64;
	ctx.unclosed = false;
	ctx.arg = (char *)malloc(sizeof(char) * ctx.cap);
	if (!ctx.arg)
	{
		_data()->early_error = true;
		return (ctx);
	}
	while (line[*i]
		&& !ft_isspace(line[*i])
		&& !ft_strchr("|<>", line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '\"')
		{
			if (!collect_quoted_ctx(&ctx, line, line[*i], i))
			{
				if (ctx.unclosed == true)
					ft_dprintf(2, "Error: Unclosed quote '%c'\n", line[*i]);
				return (ctx);
			}
		}
		else
		{
			if (!collect_plain_ctx(&ctx, line, i))
				return (ctx);
		}
	}
	if (ctx.arg)
		ctx.arg[ctx.len] = 0;
	return (ctx);
}
