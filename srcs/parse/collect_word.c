/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:41:14 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/19 12:56:30 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static bool	process_quoted_or_plain(t_ctx *ctx, const char *line,
		unsigned int *i)
{
	if (line[*i] == '\'' || line[*i] == '\"')
	{
		if (!collect_quoted_ctx(ctx, line, line[*i], i))
		{
			if (ctx->unclosed == true)
				ft_dprintf(2, "Error: Unclosed quote '%c'\n", line[*i]);
			return (false);
		}
	}
	else
	{
		if (!collect_plain_ctx(ctx, line, i))
			return (false);
	}
	return (true);
}

static bool	init_ctx(t_ctx *ctx)
{
	ft_bzero(ctx, sizeof(*ctx));
	ctx->cap = 64;
	ctx->unclosed = false;
	ctx->arg = (char *)malloc(sizeof(char) * ctx->cap);
	if (!ctx->arg)
	{
		_data()->early_error = true;
		return (false);
	}
	return (true);
}

t_ctx	collect_word_ctx(const char *line, unsigned int *i)
{
	t_ctx	ctx;

	if (!init_ctx(&ctx))
		return (ctx);
	while (line[*i]
		&& !ft_isspace(line[*i])
		&& !ft_strchr("|<>", line[*i]))
	{
		if (!process_quoted_or_plain(&ctx, line, i))
			return (ctx);
	}
	if (ctx.arg)
		ctx.arg[ctx.len] = 0;
	return (ctx);
}
