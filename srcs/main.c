/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:48:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 14:50:16 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_status = 0;

void	main_help(char **envp)
{
	_data()->env_list = env_fill(envp);
	_data()->env = env_to_array(_data()->env_list);
	get_path();
	signal_handler(0);
	g_signal_status = 0;
	_data()->flag = 0;
}

static void	main_helper(void)
{
	t_data	*data;

	data = _data();
	edit_env();
	free(data->input);
	data->input = NULL;
	free_cmdlst(data->cmds);
	data->cmds = NULL;
	free(data->prompt);
	data->prompt = NULL;
	free_split(data->env);
	data->env = env_to_array(data->env_list);
	free_split(data->path);
	data->path = NULL;
	get_path();
	data->error = false;
	data->early_error = false;
	data->pos = 0;
	data->escaped = 0;
}

static int	handle_input(void)
{
	_data()->prompt = get_prompt();
	_data()->input = readline(_data()->prompt);
	_data()->escaped = 0;
	if (!_data()->input)
	{
		printf("exit\n");
		return (0);
	}
	if (!only_space(_data()->input))
		_data()->input[0] = '\0';
	if (!*_data()->input)
	{
		free(_data()->prompt);
		free(_data()->input);
		return (1);
	}
	add_history(_data()->input);
	parsing(_data()->input);
	return (2);
}

static int	main_loop(void)
{
	int	input_status;

	input_status = handle_input();
	if (input_status == 0)
		return (0);
	if (input_status == 1)
		return (1);
	exec_cmds(_data()->cmds);
	main_helper();
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	(void)(argc);
	(void)(argv);
	main_help(envp);
	while (1)
	{
		if (!main_loop())
			break ;
	}
	ultimate_free_func();
	return (0);
}
