/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:48:33 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/10 10:49:50 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_ms_ctx(void)
{
	t_minishell	*ms;

	ms = _minishell();
	free_token_array();
	ms->error = false;
	ms->early_error = false;
	ms->pos = 0;
	ms->escaped = 0;
}

void	free_token_array(void)
{
	t_minishell	*ms;
	size_t		i;

	ms = _minishell();
	i = 0;
	if (!ms->tokens)
		return ;
	while (ms->tokens[i].text)
	{
		free(ms->tokens[i].text);
		i++;
	}
	free(ms->tokens);
}
