/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:43:32 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/19 18:26:10 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_heredocs(t_redir *r, int *tmp)
{
	int	j;

	j = 0;
	while (r)
	{
		if (r->type == TK_HEREDOC)
		{
			r->heredoc_fd = get_heredoc_fd(r->target, tmp);
			tmp[j] = r->heredoc_fd;
			if (r->heredoc_fd < 0)
				return (0);
		}
		r = r->next;
		j++;
	}
	return (1);
}

static void	close_heredocs(t_redir *r)
{
	while (r)
	{
		if (r->type == TK_HEREDOC && r->heredoc_fd >= 0)
			close(r->heredoc_fd);
		r = r->next;
	}
}

static int	handle_redir_and_files(t_cmd *cur, int *in_fd, int *tmp)
{
	ft_bzero(tmp, sizeof(int) * 1024);
	if (!check_all_files(cur->redirs))
	{
		cleanup_fds(in_fd, _data()->fd, _data()->is_last);
		_data()->exit_code = 1;
		return (0);
	}
	if (!handle_heredocs(cur->redirs, tmp))
	{
		cleanup_fds(in_fd, _data()->fd, _data()->is_last);
		return (0);
	}
	return (1);
}

static int	exec_cmds_loop(t_cmd *cmd)
{
	int		in_fd;
	t_cmd	*cur;
	int		did_fork;
	int		tmp[1024];

	in_fd = 0;
	cur = cmd;
	did_fork = 0;
	while (cur)
	{
		_data()->is_last = (cur->next == NULL);
		if (!_data()->is_last)
			pipe(_data()->fd);
		if (!handle_redir_and_files(cur, &in_fd, tmp))
		{
			cur = cur->next;
			continue ;
		}
		did_fork = exec_single_cmd(cur, &in_fd, _data()->fd, _data()->is_last);
		close_heredocs(cur->redirs);
		if (is_builtin(cur->args[0]) && !cur->next)
			break ;
		cur = cur->next;
	}
	return (did_fork);
}

void	exec_cmds(t_cmd *cmd)
{
	int	status;
	int	did_fork;

	did_fork = exec_cmds_loop(cmd);
	if (did_fork && (!_data()->early_error || !_data()->error))
		wait_for_children(&status);
}
