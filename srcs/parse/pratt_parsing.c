/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:59:59 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/23 14:53:47 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ctx	*alloc_fragments(size_t cap)
{
	t_ctx	*fragments;

	fragments = malloc(sizeof(t_ctx) * cap);
	if (!fragments)
		malloc_error();
	return (fragments);
}

bool	init_fragment(t_ctx *frag)
{
	ft_bzero(frag, sizeof(*frag));
	frag->cap = 64;
	frag->arg = malloc(frag->cap);
	if (!frag->arg)
		malloc_error();
	return (true);
}

static int	split_on_whitespace_loop(char *line, unsigned int *cap,
	unsigned int *count)
{
	unsigned int	i;
	unsigned int	size;

	i = 0;
	size = ft_strlen(line);
	while (line[i])
	{
		skip_whitespace(line, &i);
		if (!line[i])
			break ;
		if (!process_token(cap, count, line, &i))
			return (0);
		if (i >= size)
			break ;
	}
	return (1);
}

static bool	init_split_args(unsigned int *cap, t_data **minishell)
{
	*cap = 8;
	*minishell = _data();
	(*minishell)->args = malloc(sizeof(char *) * (*cap));
	(*minishell)->ctx = malloc(sizeof(t_ctx) * (*cap));
	if (!(*minishell)->ctx || !(*minishell)->args)
	{
		free((*minishell)->args);
		free((*minishell)->ctx);
		malloc_error();
	}
	ft_bzero((*minishell)->ctx, sizeof(t_ctx) * (*cap));
	ft_bzero((*minishell)->args, sizeof(char *) * (*cap));
	return (true);
}

char	**split_on_whitespace(char *line)
{
	unsigned int	cap;
	unsigned int	count;
	t_data			*minishell;

	count = 0;
	if (!init_split_args(&cap, &minishell))
		return (NULL);
	if (!split_on_whitespace_loop(line, &cap, &count))
	{
		free_split(minishell->args);
		free_ctx_str();
		free(minishell->ctx);
		minishell->args = NULL;
		minishell->ctx = NULL;
		minishell->early_error = true;
		return (NULL);
	}
	minishell->args[count] = NULL;
	minishell->ctx[count].arg = NULL;
	return (minishell->args);
}
