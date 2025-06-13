/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:38:58 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/13 11:03:02 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_ctx(void)
{
	int	i;

	i = 0;
	if (!_data()->ctx || !_data()->ctx[i].arg)
		return ;
	while (_data()->ctx[i].arg)
	{
		free(_data()->ctx[i].arg);
		_data()->ctx[i].arg = NULL;
		i++;
	}
	free(_data()->ctx);
	_data()->ctx = NULL;
}

void	parsing(char *input)
{
	int		i;
	char	*temp;

	i = 0;
	_data()->args = split_on_whitespace(input);
	if (!_data()->args || !_data()->ctx)
		return ;
	while (_data()->ctx[i].arg)
	{
		if (ft_strchr(_data()->ctx[i].arg, '$')
			&& _data()->ctx[i].is_escaped == false)
		{
			temp = _data()->ctx[i].arg;
			_data()->ctx[i].arg = replace_env_vars(temp);
			free(temp);
		}
		i++;
	}
	_data()->tokens = tokenize_to_pratt(_data()->ctx);
	_data()->ast = parse_expression(0);
	_data()->cmds = ast_to_cmd(_data()->ast);
	free_ctx();
	free_ast(_data()->ast);
	free_split(_data()->args);
	free_token_array();
}
