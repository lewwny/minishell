/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parsing_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:01:55 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 15:21:23 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	skip_whitespace(const char *line, unsigned int *i)
{
	while (line[*i] && ft_isspace(line[*i]))
		(*i)++;
}

void	join_fragments_env(t_ctx *frag)
{
	char	*tmp_env;

	if (!frag->is_escaped && ft_strchr(frag->arg, '$'))
	{
		tmp_env = frag->arg;
		frag->arg = replace_env_vars(tmp_env);
		free(tmp_env);
	}
	else if (frag->is_escaped)
		_data()->has_escaped = true;
}

t_ctx	join_fragments(t_ctx *frags, size_t count)
{
	t_ctx	joined;
	char	*result;
	size_t	i;
	char	*tmp;

	i = 0;
	result = ft_strdup("");
	_data()->has_escaped = false;
	ft_bzero(&joined, sizeof(joined));
	if (!result)
		malloc_error();
	while (i < count)
	{
		tmp = result;
		join_fragments_env(&frags[i]);
		result = ft_strjoin(result, frags[i].arg);
		free(tmp);
		free(frags[i].arg);
		i++;
	}
	joined.arg = result;
	joined.is_escaped = _data()->has_escaped;
	joined.unclosed = false;
	return (joined);
}

bool	process_token(unsigned int *cap, unsigned int *count,
		char *line, unsigned int *i)
{
	t_ctx		*frags;
	t_ctx		ctx;
	size_t		frag_count;

	if (line[*i] == '>' || line[*i] == '<' || line[*i] == '|')
		return (handle_operator(cap, count, line, i));
	frags = collect_word_fragments(line, i, &frag_count);
	if (!frags)
	{
		_data()->early_error = true;
		return (false);
	}
	ctx = join_fragments(frags, frag_count);
	free(frags);
	if (!ctx.arg)
	{
		free(ctx.arg);
		return (false);
	}
	append_t_ctx(cap, count, &ctx);
	return (true);
}
