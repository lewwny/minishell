/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:48:19 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/19 11:32:47 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_redir_in(const char *target)
{
	int	fd;

	fd = open(target, O_RDONLY);
	if (fd < 0)
		return (0);
	dup2(fd, 0);
	close(fd);
	return (1);
}

static int	handle_redir_out(const char *target)
{
	int	fd;

	fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (0);
	dup2(fd, 1);
	close(fd);
	return (1);
}

static int	handle_append(const char *target)
{
	int	fd;

	fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (0);
	dup2(fd, 1);
	close(fd);
	return (1);
}

static int	apply_single_redir(t_redir *r)
{
	if (r->type == TK_REDIR_IN)
	{
		if (!handle_redir_in(r->target))
			return (0);
	}
	else if (r->type == TK_REDIR_OUT)
	{
		if (!handle_redir_out(r->target))
			return (0);
	}
	else if (r->type == TK_APPEND)
	{
		if (!handle_append(r->target))
			return (0);
	}
	else if (r->type == TK_HEREDOC)
	{
		if (dup2(r->heredoc_fd, STDIN_FILENO) < 0)
			return (0);
		close(r->heredoc_fd);
	}
	return (1);
}

int	apply_redir(t_redir *redirs)
{
	t_redir	*r;

	r = redirs;
	while (r)
	{
		if (!apply_single_redir(r))
			return (0);
		r = r->next;
	}
	return (1);
}
