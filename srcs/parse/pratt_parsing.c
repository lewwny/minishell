/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:59:59 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/17 17:15:46 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	skip_whitespace(const char *line, unsigned int *i)
{
	while (line[*i] && ft_isspace(line[*i]))
		(*i)++;
}

static bool	process_token(unsigned int *cap, unsigned int *count, char *line,
		unsigned int *i)
{
	t_ctx	ctx;

	if (line[*i] == '>' || line[*i] == '<' || line[*i] == '|')
		return (handle_operator(cap, count, line, i));
	ctx = collect_word_ctx(line, i);
	if (_data()->early_error)
	{
		if (ctx.arg)
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
	if (!(*minishell)->ctx)
	{
		free((*minishell)->args);
		malloc_error();
		return (false);
	}
	if (!(*minishell)->args)
	{
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
		minishell->exit_status = 1;
		return (NULL);
	}
	minishell->args[count] = NULL;
	minishell->ctx[count].arg = NULL;
	return (minishell->args);
}
