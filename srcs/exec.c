/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:26:47 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/04 12:11:26 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	get_cmd(char *cmd)
{
	t_data	*data;
	int		i;

	data = _data();
	if (access(cmd, F_OK) == 0)
	{
		data->cmds->cmd_path = ft_strdup(cmd);
		return ;
	}
	i = 0;
	if (!data->path)
	{
		data->cmds->cmd_path = NULL;
		return ;
	}
	while (data->path[i])
	{
		data->cmds->cmd_path = ft_strjoin(data->path[i], cmd);
		if (access(data->cmds->cmd_path, F_OK) == 0)
			return ;
		free(data->cmds->cmd_path);
		data->cmds->cmd_path = NULL;
		i++;
	}
	data->cmds->cmd_path = NULL;
}

static void	exec(t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (data->cmds->cmd_path)
			execve(data->cmds->cmd_path, data->cmds->args, data->env);
		perror(data->cmds->args[0]);
		ultimate_free_func();
		exit(127);
	}
	else if (pid < 0)
		perror("fork");
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exit_code = WEXITSTATUS(status);
	}
}

void	exec_cmds(t_cmd *cmd)
{
	while (cmd)
	{
		if (is_builtin(cmd->args[0]))
		{
			exec_builtins(cmd);
			cmd = cmd->next;
			continue ;
		}
		get_cmd(cmd->args[0]);
		exec(_data());
		if (_data()->cmds->cmd_path)
		{
			free(_data()->cmds->cmd_path);
			_data()->cmds->cmd_path = NULL;
		}
		cmd = cmd->next;
	}
}
