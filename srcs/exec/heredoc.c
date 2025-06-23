/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:39:55 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 14:51:11 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	close_tmp_fds(int *tmp)
{
	int	j;

	j = 0;
	while (j < 1024)
	{
		if (tmp[j] > 2)
			close(tmp[j]);
		j++;
	}
}

static void	heredoc_child(const char *limiter, int *tmp, int *p)
{
	char	*line;

	signal_handler(1);
	close(p[0]);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || !ft_strcmp(line, limiter))
		{
			if (!line)
				write(2, "minishell: heredoc delimiter not found\n", 40);
			free(line);
			break ;
		}
		write(p[1], line, ft_strlen(line));
		write(p[1], "\n", 1);
		free(line);
	}
	close(p[1]);
	ultimate_free_func();
	close(_data()->fd[0]);
	close(_data()->fd[1]);
	close_tmp_fds(tmp);
	exit(0);
}

static int	heredoc_parent(pid_t pid, int *p)
{
	int	status;

	close(p[1]);
	waitpid(pid, &status, 0);
	_data()->flag = 1;
	if (WIFSIGNALED(status))
	{
		close(p[0]);
		_data()->exit_code = WTERMSIG(status) + 128;
		if (g_signal_status != 0)
		{
			_data()->exit_code = g_signal_status;
			g_signal_status = 0;
		}
		return (-1);
	}
	return (p[0]);
}

int	get_heredoc_fd(const char *limiter, int *tmp)
{
	int		p[2];
	pid_t	pid;

	if (pipe(p) < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
		heredoc_child(limiter, tmp, p);
	else
		return (heredoc_parent(pid, p));
	return (-1);
}
