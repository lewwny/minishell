/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:38:58 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/10 10:10:56 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	parsing(char *input)
{
	char	**args;
	char	*temp;
	t_cmd	*cmd;
	int		i;

	i = 0;
	args = ft_split(input, ' ');
	if (!args)
		malloc_error();
	while (args[i])
	{
		if (ft_strchr(args[i], '$'))
		{
			temp = replace_env_vars(args[i]);
			free(args[i]);
			args[i] = temp;
		}
		i++;
	}
	cmd = ft_cmdnew(args);
	if (!cmd)
		malloc_error();
	_data()->cmds = cmd;
}
