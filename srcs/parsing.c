/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:38:58 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/10 16:58:50 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	parsing(char *input)
{
	int		i;

	i = 0;
	_data()->args = split_on_whitespace(input);
	if (!_data()->args)
		malloc_error();
	while (_data()->args[i])
	{
		if (ft_strchr(_data()->args[i], '$'))
			_data()->args[i] = replace_env_vars(_data()->args[i]);
		i++;
	}
	_data()->tokens = tokenize_to_pratt(_data()->args);
	_data()->ast = parse_expression(0);
	// free_token_array();
	_data()->cmds = ast_to_cmd(_data()->ast);
	free_ast(_data()->ast);
	free_split(_data()->args);
	free_token_array();
}
