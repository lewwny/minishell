/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:48:19 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:48:28 by lengarci         ###   ########.fr       */
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

static int	apply_redir(t_redir *redir)
{
	if (redir->type == TK_REDIR_IN)
		return (handle_redir_in(redir->target));
	else if (redir->type == TK_REDIR_OUT)
		return (handle_redir_out(redir->target));
	else if (redir->type == TK_APPEND)
		return (handle_append(redir->target));
	else if (redir->type == TK_HEREDOC)
		here_doc_manage(redir);
	return (1);
}

int	apply_redirs(t_redir *redirs)
{
	while (redirs)
	{
		if (!apply_redir(redirs))
			return (0);
		redirs = redirs->next;
	}
	return (1);
}
