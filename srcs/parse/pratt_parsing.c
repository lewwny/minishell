/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:59:59 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/19 12:20:19 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ensure_str_capacity(char **buf, unsigned int *cap, unsigned int needed)
{
	unsigned int	new_cap;
	char			*new_buf;

	if (needed >= *cap)
	{
		new_cap = *cap * 2;
		new_buf = ft_realloc(*buf, *cap, new_cap);
		if (!new_buf)
			return (false);
		*buf = new_buf;
		*cap = new_cap;
	}
	return (true);
}

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
		return (false);
	return (true);
}

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

static bool	add_fragment(t_ctx **fragments, size_t *idx, size_t *cap,
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

static bool	collect_one_fragment(const char *line, unsigned int *i, t_ctx *frag)
{
	if (!init_fragment(frag))
		return (false);
	if (line[*i] == '\'' || line[*i] == '"')
	{
		if (!handle_quoted(line, i, frag))
		{
			free(frag->arg);
			return (false);
		}
	}
	else
	{
		if (!handle_unquoted(line, i, frag))
		{
			free(frag->arg);
			return (false);
		}
	}
	frag->arg[frag->len] = '\0';
	return (true);
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
	while (line[*i] && !ft_isspace(line[*i]) && !ft_strchr("|<>", line[*i]))
	{
		if (!collect_one_fragment(line, i, &frag))
		{
			free(fragments);
			return (NULL);
		}
		if (!add_fragment(&fragments, &idx, &cap, &frag))
			return (NULL);
	}
	*count = idx;
	return (fragments);
}

static void	skip_whitespace(const char *line, unsigned int *i)
{
	while (line[*i] && ft_isspace(line[*i]))
		(*i)++;
}

static void	join_fragments_env(t_ctx *frag)
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

static t_ctx	join_fragments(t_ctx *frags, size_t count)
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

static bool	process_token(unsigned int *cap, unsigned int *count,
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
		return (false);
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
