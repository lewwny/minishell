/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:48:33 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/15 14:53:33 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_ms_ctx(void)
{
	t_data	*data;

	data = _data();
	free_token_array();
	data->error = false;
	data->early_error = false;
	data->pos = 0;
	data->escaped = 0;
}

static void	free_redir_list(t_redir *r)
{
	t_redir	*redir_next;

	while (r)
	{
		redir_next = r->next;
		free(r->target);
		free(r);
		r = redir_next;
	}
}

void	free_cmdlst(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->args)
			free_split(cmd->args);
		cmd->args = NULL;
		free_redir_list(cmd->redirs);
		free(cmd);
		cmd = next;
	}
}
