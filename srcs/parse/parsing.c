/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:38:58 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 14:43:10 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static void	expand_env_vars_in_ctx(void)
{
	int		i;
	char	*temp;

	i = 0;
	if (!_data()->ctx)
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
}

void	parsing(char *input)
{
	_data()->escaped = 0;
	_data()->args = split_on_whitespace(input);
	if (!_data()->args || !_data()->ctx || _data()->early_error)
	{
		_data()->exit_code = 1;
		free_ctx();
		ft_dprintf(STDERR_FILENO, "Error: An error occurred during parsing\n");
		return ;
	}
	expand_env_vars_in_ctx();
	_data()->tokens = tokenize_to_pratt(_data()->ctx);
	_data()->ast = parse_expression(0);
	if (_data()->early_error || _data()->error)
	{
		_data()->exit_code = 1;
		free_ast(_data()->ast);
		_data()->ast = NULL;
	}
	if (_data()->ast)
		_data()->cmds = ast_to_cmd(_data()->ast);
	free_ctx();
	free_ast(_data()->ast);
	free_split(_data()->args);
	free_token_array();
}
