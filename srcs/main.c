/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:48:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/03 17:23:36 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

void	end_program(void)
{
	rl_clear_history();
	if (_data()->path)
		free_split(_data()->path);
}

void	main_help(char **envp)
{
	get_path();
	_data()->env_list = env_fill(envp);
	_data()->env = env_to_array(_data()->env_list);
}

static void main_helper(void)
{
	free_split(_data()->path);
	get_path();
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	(void)argc;
	(void)argv;
	data = _data();
	main_help(envp);
	while (1)
	{
		data->input = readline(COLOR_GREEN "minishell$>" COLOR_RESET);
		if (!data->input)
		{
			printf("exit\n");
			break ;
		}
		if (!only_space(data->input))
			data->input[0] = '\0';
		if (!*data->input)
			continue ;
		add_history(data->input);
		parsing(data->input);
		if (ft_strcmp(data->input, "/test") == 0)
			print_env_list(data->env_list);
		exec_cmds(_data()->cmds);
		free_cmd();
		main_helper();
	}
	end_program();
	return (0);
}
