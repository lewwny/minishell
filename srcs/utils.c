/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 08:40:27 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/03 18:23:50 by lengarci         ###   ########.fr       */
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

void	print_env_list(t_env *env)
{
	while (env)
	{
		if (env->key && env->value)
		{
			printf("%s=%s\n", env->key, env->value);
		}
		env = env->next;
	}
}

void	delete_env_node(t_env **env, const char *key)
{
	t_env	*curr;
	t_env	*prev;

	if (!env || !*env || !key)
		return ;
	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			if (curr->value)
				free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
