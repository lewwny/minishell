/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_ctx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:06:55 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/20 15:35:22 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static size_t	get_ctx_args_size(t_ctx *ctx)
{
	size_t	size;

	size = 0;
	if (!ctx || !ctx->arg)
		return (0);
	while (_data()->args[size])
		size++;
	return (size);
}

static void	*alloc_args_array(size_t size)
{
	char	**args;

	args = malloc(sizeof(char *) * (size + 1));
	if (!args)
		malloc_error();
	return (args);
}

static void	fill_args_array(char **args, t_ctx *ctx, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size && ctx[i].arg)
	{
		args[i] = ft_strdup(ctx[i].arg);
		if (!args[i])
			malloc_error();
		i++;
	}
	args[i] = NULL;
}

char	**get_args_from_ctx(t_ctx *ctx)
{
	char	**args;
	size_t	size;

	size = get_ctx_args_size(ctx);
	if (size == 0)
		return (NULL);
	args = alloc_args_array(size);
	fill_args_array(args, ctx, size);
	return (args);
}
