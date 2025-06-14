/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:19:39 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/13 11:17:55 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static unsigned int	copy_quoted_content(const char *str, char quote,
		unsigned int end, t_ctx *buff)
{
	unsigned int	j;
	unsigned int	p;
	char			*temp;

	j = 1;
	p = 0;
	while (j < end)
	{
		if (str[j] == '\\' && quote == '"' && str[j + 1]
			&& ft_strchr("\"\\$`", str[j + 1]))
		{
			_data()->escaped++;
			if (str[j + 1] == '$')
				buff->is_escaped = true;
			j++;
			buff->arg[p++] = str[j++];
		}
		else if (str[j] == '$' && quote == '\'')
		{
			_data()->escaped++;
			buff->is_escaped = true;
			buff->arg[p++] = str[j++];
		}
		else
			buff->arg[p++] = str[j++];
	}
	buff->arg[p] = '\0';
	if (!buff->is_escaped && ft_strchr(buff->arg, '$'))
	{
		temp = buff->arg;
		buff->arg = replace_env_vars(temp);
		free(temp);
	}
	return (p);
}

static unsigned int	find_closing_quote(const char *str, char quote)
{
	unsigned int	i;

	i = 1;
	while (str[i] && str[i] != quote)
	{
		if (str[i] == '\\' && quote == '"' && str[i + 1] && ft_strchr("\"\\$`", str[i + 1]))
			i += 2;
		else
			i++;
	}
	return (i);
}

t_ctx	collect_quoted(const char *str, char quote, bool *unclosed)
{
	unsigned int	end;
	t_ctx			buff;

	//buff = (t_ctx *)malloc(sizeof(t_ctx));
	ft_bzero(&buff, sizeof(buff));
	end = find_closing_quote(str, quote);
	if (str[end] != quote)
	{
		*unclosed = true;
		buff.arg = NULL;
		return (buff);
	}
	buff.arg = (char *)malloc(end);
	if (!buff.arg)
	{
		free_split(_data()->args);
		return (buff);
	}
	copy_quoted_content(str, quote, end, &buff);
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
