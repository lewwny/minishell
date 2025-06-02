/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:38:58 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 17:52:35 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	parsing(char *input)
{
	char	**args;
	t_cmd	*cmd;

	args = ft_split(input, ' ');
	if (!args)
		malloc_error();
	cmd = ft_cmdnew(args);
	if (!cmd)
		malloc_error();
	_data()->cmds = cmd;
}
