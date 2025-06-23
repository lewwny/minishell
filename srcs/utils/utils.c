/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 08:40:27 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/20 15:31:30 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	*ft_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		malloc_error();
	ft_bzero(new_ptr, new_size);
	if (ptr)
	{
		if (old_size < new_size)
			ft_memcpy(new_ptr, ptr, old_size);
		else
			ft_memcpy(new_ptr, ptr, new_size);
		free(ptr);
	}
	return (new_ptr);
}

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
		malloc_error();
	cmd->cmd_path = NULL;
	cmd->args = content;
	cmd->next = NULL;
	cmd->redirs = NULL;
	return (cmd);
}
