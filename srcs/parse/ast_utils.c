/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:53:58 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/19 13:19:15 by macauchy         ###   ########.fr       */
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

static size_t	count_args(char **src)
{
	size_t	n;

	n = 0;
	if (src)
	{
		while (src[n])
			n++;
	}
	return (n);
}

static int	copy_args_content(char **dst, char **src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
		{
			free_split(dst);
			return (0);
		}
		i++;
	}
	dst[n] = NULL;
	return (1);
}

char	**copy_args(char **src)
{
	size_t	n;
	char	**dst;

	n = count_args(src);
	dst = (char **)malloc(sizeof(char *) * (n + 1));
	if (!dst)
		return (NULL);
	if (!copy_args_content(dst, src, n))
		return (NULL);
	return (dst);
}
