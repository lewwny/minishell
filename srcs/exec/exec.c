/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:26:47 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/22 11:59:38 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	setup_child_io(int in_fd, int out_fd, t_data *data)
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
}

void	exec_child(t_cmd *cmd, int in_fd, int out_fd, t_data *data)
{
	setup_child_io(in_fd, out_fd, data);
	if (!apply_redir(cmd->redirs))
	{
		ultimate_free_func();
		write(2, "minishell: redirection error\n", 30);
		exit(1);
	}
	get_cmd(cmd->args[0]);
	if (is_builtin(cmd->args[0]))
	{
		exec_builtins(cmd);
		ultimate_free_func();
		exit(0);
	}
	if (data->cmds->cmd_path)
		execve(data->cmds->cmd_path, cmd->args, data->env);
	write(2, cmd->args[0], ft_strlen(cmd->args[0]));
	write(2, ": command not found\n", 21);
	ultimate_free_func();
	exit(127);
}

int	check_all_files(t_redir *redirs)
{
	t_redir	*r;
	int		fd;

	r = redirs;
	while (r)
	{
		if (r->type == TK_REDIR_IN)
		{
			fd = open(r->target, O_RDONLY);
			if (fd < 0)
			{
				perror(r->target);
				return (0);
			}
			close(fd);
		}
		r = r->next;
	}
	return (1);
}
