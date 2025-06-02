/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:48:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 17:56:46 by lengarci         ###   ########.fr       */
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
	_data()->env = envp;
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	*data;

	(void)argc;
	(void)argv;
	data = _data();
	main_help(envp);
	while (1)
	{
		input = readline(COLOR_GREEN "minishell$>" COLOR_RESET);
		if (!input || ft_strncmp(input, "exit", 5) == 0)
		{
			if (input)
				free(input);
			printf("exit\n");
			break ;
		}
		if (!only_space(input))
			input[0] = '\0';
		if (!*input)
			continue ;
		add_history(input);
		parsing(input);
		if (ft_strncmp(input, "/test", 5) == 0)
			print_tab(_data()->path);
		exec_cmds(_data()->cmds);
		free_cmd(input);
	}
	end_program();
	return (0);
}
