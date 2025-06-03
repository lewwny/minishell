/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:26:47 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/03 17:19:41 by lengarci         ###   ########.fr       */
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
	while (data->path[i])
	{
		data->cmds->cmd_path = ft_strjoin(data->path[i], cmd);
		if (access(data->cmds->cmd_path, F_OK) == 0)
			return ;
		free(data->cmds->cmd_path);
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
		print_tab(data->env);
		if (execve(data->cmds->cmd_path, data->cmds->args, data->env) == -1)
		{
			perror(data->cmds->args[0]);
			exit(1);
		}
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
			free(_data()->cmds->cmd_path);
		cmd = cmd->next;
	}
}
