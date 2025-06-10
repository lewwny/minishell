/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:59:59 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/10 15:45:29 by macauchy         ###   ########.fr       */
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
	t_data			*minishell;

	i = 0;
	minishell = _data();
	while (line[i])
	{
		skip_whitespace(line, &i);
		if (!line[i])
			break ;
		if (!process_token(cap, count, line, &i))
		{
			free(line);
			free_split(minishell->args);
			minishell->args = NULL;
			return (0);
		}
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
	if (!minishell->args)
		return (NULL);
	ft_bzero(minishell->args, sizeof(char *) * cap);
	if (!split_on_whitespace_loop(line, &cap, &count))
		return (NULL);
	minishell->args[count] = NULL;
	return (minishell->args);
}
