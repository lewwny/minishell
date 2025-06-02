/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 08:40:27 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 17:50:06 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
}

int	only_space(char *str)
{
	int	i;
	int	letter;

	letter = 0;
	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if ((str[i] != 32) && (str[i] < 9 || str[i] > 13))
			letter++;
		i++;
	}
	return (letter);
}

t_cmd	*ft_cmdnew(char **content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *) malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = content;
	cmd->next = NULL;
	cmd->redir = NULL;
	return (cmd);
}
