/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:05:59 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/05 14:00:17 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

static void	echo_builtin(t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->args[1] && ft_strncmp(cmd->args[1], "-n", 2) == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstri(1, cmd->args[i]);
		if (cmd->args[i + 1])
			ft_putchari(1, ' ');
		i++;
	}
	if (newline)
		ft_putchari(1, '\n');
}

void	exec_builtins(t_cmd *cmd)
{
	if (ft_strcmp(cmd->args[0], "env") == 0)
		env_builtin();
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		exit_builtin();
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		pwd_builtin();
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		cd_builtin();
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		unset_builtin();
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		export_builtin();
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		echo_builtin(_data()->cmds);
}
