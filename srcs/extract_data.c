/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:19:39 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/10 12:21:56 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static unsigned int	copy_quoted_content(const char *str, char quote, \
		unsigned int end, char *buff)
{
	unsigned int	j;
	unsigned int	p;

	j = 1;
	p = 0;
	while (j < end)
	{
		if (str[j] == '\\' && quote == '"' && str[j + 1] && strchr("\"\\$`", str[j + 1]))
		{
			_data()->escaped++;
			j++;
			buff[p++] = str[j++];
		}
		else
			buff[p++] = str[j++];
	}
	buff[p] = '\0';
	return (p);
}

static unsigned int	find_closing_quote(const char *str, char quote)
{
	unsigned int	i;

	i = 1;
	while (str[i] && str[i] != quote)
	{
		if (str[i] == '\\' && quote == '"' && str[i + 1] && str[i + 1] == '"')
			i += 2;
		else
			i++;
	}
	return (i);
}

char	*collect_quoted(const char *str, char quote, bool *unclosed)
{
	unsigned int	end;
	char			*buff;

	buff = NULL;
	end = find_closing_quote(str, quote);
	if (str[end] != quote)
	{
		*unclosed = true;
		return (NULL);
	}
	buff = (char *)malloc(end);
	if (!buff)
	{
		free_split(_data()->args);
		return (NULL);
	}
	copy_quoted_content(str, quote, end, buff);
	return (buff);
}

char	*extract_operator(const char *line, unsigned int *i)
{
	unsigned int	op_len;
	char			*op_token;

	op_len = 1;
	op_token = NULL;
	if ((line[*i] == '>' && line[*i + 1] == '>')
		|| (line[*i] == '<' && line[*i + 1] == '<')
		|| (line[*i] == '|' && line[*i + 1] == '|')
		|| (line[*i] == '&' && line[*i + 1] == '&')
		|| (line[*i] == '>' && line[*i + 1] == '&')
		|| (line[*i] == '<' && line[*i + 1] == '&'))
		op_len = 2;
	op_token = (char *)malloc(op_len + 1);
	if (!op_token)
		return (NULL);
	ft_memcpy(op_token, line + *i, op_len);
	op_token[op_len] = '\0';
	*i += op_len;
	return (op_token);
}

char	*extract_word(const char *line, unsigned int *i)
{
	unsigned int	start;
	unsigned int	len;
	char			*token;

	len = 0;
	start = *i;
	token = NULL;
	while (line[*i] && !ft_isspace(line[*i])
		&& line[*i] != '>' && line[*i] != '<'
		&& line[*i] != '|' && line[*i] != '(' && line[*i] != ')'
		&& line[*i] != ';' && line[*i] != '\'' && line[*i] != '"')
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (NULL);
	token = (char *)malloc(len + 1);
	if (!token)
		return (NULL);
	ft_memcpy(token, line + start, len);
	token[len] = '\0';
	return (token);
}
