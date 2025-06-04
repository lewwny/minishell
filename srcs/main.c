/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:48:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/04 12:04:56 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

void	main_help(char **envp)
{
	_data()->env_list = env_fill(envp);
	_data()->env = env_to_array(_data()->env_list);
	get_path();
}

static void	main_helper(void)
{
	free(_data()->input);
	_data()->input = NULL;
	free_split(_data()->env);
	_data()->env = env_to_array(_data()->env_list);
	ft_cmdclear(&_data()->cmds);
	free_split(_data()->path);
	_data()->path = NULL;
	get_path();
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	main_help(envp);
	while (1)
	{
		_data()->input = readline(COLOR_GREEN "minishell$>" COLOR_RESET);
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
		if (ft_strcmp(_data()->input, "/test") == 0)
			print_env_list(_data()->env_list);
		exec_cmds(_data()->cmds);
		main_helper();
	}
	ultimate_free_func();
	return (0);
}
