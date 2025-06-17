/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:53:58 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/17 14:33:37 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->next = NULL;
	cmd->redirs = NULL;
	cmd->cmd_path = NULL;
	return (cmd);
}

char	**copy_args(char **src)
{
	size_t	n;
	size_t	i;
	char	**dst;

	n = 0;
	i = 0;
	if (src)
	{
		while (src[n])
			n++;
	}
	dst = (char **)malloc(sizeof(char *) * (n + 1));
	if (!dst)
		return (NULL);
	while (i < n)
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
		{
			free_split(dst);
			return (NULL);
		}
		i++;
	}
	dst[n] = NULL;
	return (dst);
}
