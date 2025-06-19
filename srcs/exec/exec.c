/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:26:47 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/19 10:27:14 by lengarci         ###   ########.fr       */
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

int get_heredoc_fd(const char *limiter, int *tmp)
{
	int		p[2];
	pid_t	pid;
	int		status;
	int		j;

	if (pipe(p) < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		signal_handler(1);
		close(p[0]);
		j = 0;
		while (1)
		{
			char *line = readline("heredoc> ");
			if (!line || !ft_strcmp(line, limiter))
			{
				if (!line)
					write(2, "minishell: heredoc delimiter not found\n", 40);
				free(line);
				break;
			}
			write(p[1], line, ft_strlen(line));
			write(p[1], "\n", 1);
			free(line);
		}
		close(p[1]);
		ultimate_free_func();
		close(_data()->fd[0]);
		close(_data()->fd[1]);
		while (j < 1024)
		{
			if (tmp[j] > 2)
				close(tmp[j]);
			j++;
		}
		exit(0);
	}
	else
	{
		close(p[1]);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			close(p[0]);
			_data()->exit_code = 130;
			return (-1);
		}
		return (p[0]);
	}
}

static int check_all_files(t_redir *redirs)
{
    t_redir *r = redirs;
    while (r)
    {
        if (r->type == TK_REDIR_IN)
        {
            int fd = open(r->target, O_RDONLY);
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

void exec_cmds(t_cmd *cmd)
{
	int   in_fd  = 0;
	t_cmd *cur   = cmd;
	int   status;
	int   did_fork = 0;
	int		j;

	while (cur)
	{
		_data()->is_last = (cur->next == NULL);
		if (!_data()->is_last)
			pipe(_data()->fd);
		t_redir *r = cur->redirs;
		int tmp[1024];
		ft_bzero(&tmp, sizeof(tmp));
		j = 0;
		if (!check_all_files(cur->redirs))
		{
			cleanup_fds(&in_fd, _data()->fd, _data()->is_last);
			_data()->exit_code = 1;
			cur = cur->next;
			continue; // Passe à la commande suivante
		}
		while (r)
		{
			if (r->type == TK_HEREDOC)
			{
				r->heredoc_fd = get_heredoc_fd(r->target, tmp);
				tmp[j] = r->heredoc_fd;
				if (r->heredoc_fd < 0)
				{
					cleanup_fds(&in_fd, _data()->fd, _data()->is_last);
					return;
				}
			}
			r = r->next;
			j++;
		}
		did_fork = exec_single_cmd(cur, &in_fd, _data()->fd, _data()->is_last);
		r = cur->redirs;
		while (r)
		{
			if (r->type == TK_HEREDOC && r->heredoc_fd >= 0)
				close(r->heredoc_fd);
			r = r->next;
		}
		if (is_builtin(cur->args[0]) && !cur->next)
			break;
		cur = cur->next;
	}
	if (did_fork && (!_data()->early_error || !_data()->error))
		wait_for_children(&status);
}
