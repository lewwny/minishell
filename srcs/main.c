/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:48:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/10 17:15:32 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	main_help(char **envp)
{
	_data()->env_list = env_fill(envp);
	_data()->env = env_to_array(_data()->env_list);
	get_path();
	signal_handler(0);
}

static void	main_helper(void)
{
	t_data	*data;

	data = _data();
	free(data->input);
	data->input = NULL;
	free(data->prompt);
	data->prompt = NULL;
	free_split(data->env);
	data->env = env_to_array(data->env_list);
	// free_cmdlst(data->cmds);
	// ft_cmdclear(&data->cmds);
	free_split(data->path);
	data->path = NULL;
	get_path();
	// free_token_array();
	data->error = false;
	data->early_error = false;
	data->pos = 0;
	data->escaped = 0;
}

int	main(int argc, char **argv, char **envp)
{
	(void)(argc);
	(void)(argv);
	main_help(envp);
	while (1)
	{
		_data()->prompt = get_prompt();
		_data()->input = readline(_data()->prompt);
		if (!_data()->input)
		{
			printf("exit\n");
			break ;
		}
		if (!only_space(_data()->input))
			_data()->input[0] = '\0';
		if (!*_data()->input)
			continue ;
		add_history(_data()->input);
		parsing(_data()->input);
		exec_cmds(_data()->cmds);
		main_helper();
	}
	ultimate_free_func();
	return (0);
}
