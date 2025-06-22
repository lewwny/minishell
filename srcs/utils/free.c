/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:28:14 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/22 12:01:44 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

static void	free_data_members(void)
{
	if (_data()->cmds && _data()->cmds->cmd_path)
	{
		free(_data()->cmds->cmd_path);
		_data()->cmds->cmd_path = NULL;
	}
	if (_data()->cmds && _data()->cmds->args)
	{
		free_split(_data()->cmds->args);
		_data()->cmds->args = NULL;
	}
	if (_data()->path)
	{
		free_split(_data()->path);
		_data()->path = NULL;
	}
	if (_data()->env)
	{
		free_split(_data()->env);
		_data()->env = NULL;
	}
	if (_data()->input)
	{
		free(_data()->input);
		_data()->input = NULL;
	}
}

static void	free_data_lists(void)
{
	if (_data()->env_list)
	{
		free_env_list(_data()->env_list);
		_data()->env_list = NULL;
	}
	if (_data()->cmds)
	{
		free_cmdlst(_data()->cmds);
		_data()->cmds = NULL;
	}
	if (_data()->prompt)
	{
		free(_data()->prompt);
		_data()->prompt = NULL;
	}
	rl_clear_history();
}

void	ultimate_free_func(void)
{
	free_data_members();
	free_data_lists();
}
