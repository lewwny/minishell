/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:25:13 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/10 15:35:55 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_child_process(t_cmd *cur, int in_fd, int *fd, int is_last)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (!is_last)
			exec_child(cur, in_fd, fd[1], _data());
		else
			exec_child(cur, in_fd, 1, _data());
	}
	else if (pid < 0)
		perror("fork");
}

void	cleanup_fds(int *in_fd, int *fd, int is_last)
{
	if (*in_fd != 0)
		close(*in_fd);
	if (!is_last)
	{
		close(fd[1]);
		*in_fd = fd[0];
	}
	else
		*in_fd = 0;
}

void	free_cmd_path(void)
{
	if (_data()->cmds->cmd_path)
	{
		free(_data()->cmds->cmd_path);
		_data()->cmds->cmd_path = NULL;
	}
}

void	wait_for_children(int *status)
{
	while (wait(status) > 0)
		;
	if (WIFEXITED(*status))
		_data()->exit_code = WEXITSTATUS(*status);
	else
		_data()->exit_code = 1;
}

void	exec_single_cmd(t_cmd *cur, int *in_fd, int *fd, int is_last)
{
	get_cmd(cur->args[0]);
	if (is_builtin(cur->args[0]) && !cur->next)
	{
		exec_builtins(cur);
		free_cmd_path();
		return ;
	}
	handle_child_process(cur, *in_fd, fd, is_last);
	cleanup_fds(in_fd, fd, is_last);
	free_cmd_path();
}
