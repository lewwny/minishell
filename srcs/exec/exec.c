/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:26:47 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/18 11:28:35 by lengarci         ###   ########.fr       */
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
	if (!apply_redirs(cmd->redirs))
	{
		ultimate_free_func();
		perror("minishell: redirection error");
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
	perror(cmd->args[0]);
	ultimate_free_func();
	exit(127);
}


int  get_heredoc_fd(const char *limiter)
{
	int p[2];
	char *line;

	if (pipe(p) < 0)
		return (-1);
	signal_handler(1);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || !ft_strcmp(line, limiter))
		{
			free(line);
			write(2, "minishell: heredoc delimiter not found\n", 40);
			break;
		}
		ft_putendl_fd(line, p[1]);
		free(line);
	}
	signal_handler(0);
	close(p[1]);
	return (p[0]);
}

void exec_cmds(t_cmd *cmd)
{
	int   in_fd  = 0;
	t_cmd *cur   = cmd;
	int   status;
	int		did_fork;

	while (cur)
	{
		_data()->is_last = (cur->next == NULL);
		if (!_data()->is_last)
			pipe(_data()->fd);
		for (t_redir *r = cur->redirs; r; r = r->next)
		{
			if (r->type == TK_HEREDOC)
			{
				r->heredoc_fd = get_heredoc_fd(r->target);
				if (r->heredoc_fd < 0)
				{
					perror("heredoc");
					exit(1);
				}
			}
		}
		did_fork = exec_single_cmd(cur, &in_fd, _data()->fd, _data()->is_last);
		if (is_builtin(cur->args[0]) && !cur->next)
			break ;
		cur = cur->next;
	}
	if (did_fork)
		wait_for_children(&status);
}
