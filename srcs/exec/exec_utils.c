/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:25:13 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 14:52:42 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_child_process(t_cmd *cur, int in_fd, int *fd, int is_last)
{
	pid_t	pid;

	pid = fork();
	signal_handler(2);
	if (!_data()->flag)
		g_signal_status = 0;
	else
		_data()->flag = 0;
	if (pid == 0)
	{
		if (!is_last)
			exec_child(cur, in_fd, fd[1], _data());
		else
			exec_child(cur, in_fd, 1, _data());
	}
	else if (pid < 0)
		perror("fork");
	signal_handler(0);
	_data()->pid = pid;
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
	{
		if (fd[0] != 0)
			close(fd[0]);
		*in_fd = 0;
	}
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
	waitpid(_data()->pid, status, 0);
	while (wait(NULL) > 0)
		;
	if (_data()->pid)
	{
		if (WIFSIGNALED(*status))
			_data()->exit_code = 128 + WTERMSIG(*status);
		else
			_data()->exit_code = WEXITSTATUS(*status);
	}
	if (g_signal_status != 0)
	{
		_data()->exit_code = g_signal_status;
		g_signal_status = 0;
	}
}

int	exec_single_cmd(t_cmd *cur, int *in_fd, int *fd, int is_last)
{
	struct stat	st;

	if (stat(cur->args[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		write(2, "minishell: is a directory\n", 27);
		_data()->exit_code = 126;
		free_cmd_path();
		return (0);
	}
	get_cmd(cur->args[0]);
	if ((ft_strcmp(cur->args[0], "exit") == 0
			|| ft_strcmp(cur->args[0], "cd") == 0
			|| ft_strcmp(cur->args[0], "unset") == 0
			|| ft_strcmp(cur->args[0], "export") == 0) && !cur->next)
	{
		free_cmd_path();
		exec_builtins(cur);
		return (0);
	}
	handle_child_process(cur, *in_fd, fd, is_last);
	cleanup_fds(in_fd, fd, is_last);
	free_cmd_path();
	return (1);
}
