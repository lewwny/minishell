/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:26:47 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:50:59 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	here_doc_manage(t_redir *redir)
{
	int		p[2];
	char	*heredoc_input;

	pipe(p);
	signal_handler(1);
	while (1)
	{
		heredoc_input = readline("heredoc> ");
		if (!heredoc_input || !ft_strcmp(heredoc_input, redir->target))
		{
			if (!heredoc_input && !(_data()->exit_code == 130))
				ft_dprintf(2, "minishell: warning: here-document wanted `%s'\n",
					redir->target);
			free(heredoc_input);
			break ;
		}
		write(p[1], heredoc_input, ft_strlen(heredoc_input));
		write(p[1], "\n", 1);
		free(heredoc_input);
	}
	close(p[1]);
	if (!(_data()->exit_code == 130))
		dup2(p[0], 0);
	close(p[0]);
	signal_handler(0);
}

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
