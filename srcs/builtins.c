/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:05:59 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/03 18:19:10 by lengarci         ###   ########.fr       */
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
}
