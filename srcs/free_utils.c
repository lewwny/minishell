/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:55:36 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/04 10:57:27 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_split(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	ft_cmdclear(t_cmd **cmd)
{
	t_cmd	*tmp;

	while (*cmd)
	{
		tmp = (*cmd)->next;
		free_split((*cmd)->args);
		free(*cmd);
		*cmd = tmp;
	}
	*cmd = NULL;
}
