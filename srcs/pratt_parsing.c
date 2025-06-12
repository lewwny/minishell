/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:59:59 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/12 17:12:58 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	skip_whitespace(const char *line, unsigned int *i)
{
	while (line[*i] && ft_isspace(line[*i]))
		(*i)++;
}

static bool	process_token(unsigned int *cap, unsigned int *count, char *line,
		unsigned int *i)
{
	if (line[*i] == '\'' || line[*i] == '"')
	{
		if (!handle_quote(cap, count, line, i))
			return (false);
	}
	else
	{
		handle_word(cap, count, line, i);
		if (line[*i] == '>' || line[*i] == '<' || line[*i] == '|'
			|| line[*i] == '(' || line[*i] == ')' || line[*i] == ';'
			|| line[*i] == '&')
		{
			if (!handle_operator(cap, count, line, i))
				return (false);
		}
	}
	return (true);
}

static int	split_on_whitespace_loop(char *line, unsigned int *cap, unsigned int *count)
{
	unsigned int	i;

	i = 0;
	while (line[i])
	{
		skip_whitespace(line, &i);
		if (!line[i])
			break ;
		if (!process_token(cap, count, line, &i))
			return (0);
	}
	return (1);
}

char	**split_on_whitespace(char *line)
{
	unsigned int	cap;
	unsigned int	count;
	t_data			*minishell;

	cap = 8;
	count = 0;
	minishell = _data();
	minishell->args = malloc(sizeof(char *) * (cap));
	minishell->ctx = malloc(sizeof(t_ctx) * (cap));
	ft_bzero(minishell->ctx, sizeof(t_ctx) * (cap));
	if (!minishell->ctx)
	{
		free(minishell->args);
		malloc_error();
	}
	if (!minishell->args)
		malloc_error();
	ft_bzero(minishell->args, sizeof(char *) * cap);
	if (!split_on_whitespace_loop(line, &cap, &count))
	{
		free_split(minishell->args);
		return (NULL);
	}
	minishell->args[count] = NULL;
	minishell->ctx[count].arg = NULL;
	return (minishell->args);
}
