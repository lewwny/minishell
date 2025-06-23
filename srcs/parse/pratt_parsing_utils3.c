/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parsing_utils3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:22:01 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/23 15:22:07 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_fragments_on_error(t_ctx *fragments, size_t idx, t_ctx *frag)
{
	size_t	j;

	j = 0;
	while (j < idx)
	{
		free(fragments[j].arg);
		j++;
	}
	free(fragments);
	free(frag->arg);
	frag->arg = NULL;
}

static bool	is_fragment_delimiter(char c)
{
	return (ft_isspace(c) || ft_strchr("|<>", c));
}

t_ctx	*collect_word_fragments(const char *line, unsigned int *i,
		size_t *count)
{
	size_t	cap;
	size_t	idx;
	t_ctx	frag;
	t_ctx	*fragments;

	cap = 4;
	idx = 0;
	fragments = alloc_fragments(cap);
	if (!fragments)
		return (NULL);
	while (line[*i] && !is_fragment_delimiter(line[*i]))
	{
		if (!collect_one_fragment(line, i, &frag))
		{
			free_fragments_on_error(fragments, idx, &frag);
			return (NULL);
		}
		if (!add_fragment(&fragments, &idx, &cap, &frag))
			return (NULL);
	}
	*count = idx;
	return (fragments);
}
