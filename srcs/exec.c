/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:26:47 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/10 19:37:36 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_cmd(char *cmd)
{
	t_data	*data;
	int		i;

	data = _data();
	if (access(cmd, F_OK) == 0)
	{
		data->cmds->cmd_path = ft_strdup(cmd);
		if (!data->cmds->cmd_path)
			malloc_error();
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
		if (!data->cmds->cmd_path)
			malloc_error();
		if (access(data->cmds->cmd_path, F_OK) == 0)
			return ;
		free(data->cmds->cmd_path);
		data->cmds->cmd_path = NULL;
		i++;
	}
	data->cmds->cmd_path = NULL;
}

void	exec_child(t_cmd *cmd, int in_fd, int out_fd, t_data *data)
{
	if (in_fd != 0)
	{
		dup2(in_fd, 0);
		close(in_fd);
	}
	if (out_fd != 1)
	{
		dup2(out_fd, 1);
		close(out_fd);
	}
	if (!data->is_last)
		close(_data()->fd[0]);
	get_cmd(cmd->args[0]);
	if (is_builtin(cmd->args[0]))
	{
		exec_builtins(cmd);
		ultimate_free_func();
		exit(0);
	}
	if (data->cmds->cmd_path)
		execve(data->cmds->cmd_path, cmd->args, data->env);
	perror(cmd->args[0]);
	ultimate_free_func();
	exit(127);
}

void	exec_cmds(t_cmd *cmd)
{
	int		in_fd;
	t_cmd	*cur;
	int		status;

	in_fd = 0;
	cur = cmd;
	status = 0;
	while (cur)
	{
		_data()->is_last = (cur->next == NULL);
		if (!_data()->is_last)
			pipe(_data()->fd);
		exec_single_cmd(cur, &in_fd, _data()->fd, _data()->is_last);
		if (is_builtin(cur->args[0]) && !cur->next)
			break ;
		cur = cur->next;
	}
	wait_for_children(&status);
}
