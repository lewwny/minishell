/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:26:47 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/11 10:17:51 by lengarci         ###   ########.fr       */
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

static void	here_doc_manage(t_redir *redir)
{
	int		p[2];
	char	*heredoc_input;

	pipe(p);
	while (1)
	{
		heredoc_input = readline("heredoc> ");
		if (!heredoc_input || !ft_strcmp(heredoc_input, redir->target))
		{
			if (!heredoc_input)
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
	dup2(p[0], 0);
	close(p[0]);
}

static void	apply_redirs(t_redir *redirs)
{
	int	fd;

	while (redirs)
	{
		if (redirs->type == TK_REDIR_IN)
		{
			fd = open(redirs->target, O_RDONLY);
			if (fd < 0)
				perror("open");
			dup2(fd, 0);
			close(fd);
		}
		else if (redirs->type == TK_REDIR_OUT)
		{
			fd = open(redirs->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				perror("open");
			dup2(fd, 1);
			close(fd);
		}
		else if (redirs->type == TK_APPEND)
		{
			fd = open(redirs->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				perror("open");
			dup2(fd, 1);
			close(fd);
		}
		else if (redirs->type == TK_HEREDOC)
			here_doc_manage(redirs);
		redirs = redirs->next;
	}
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
	apply_redirs(cmd->redirs);
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
