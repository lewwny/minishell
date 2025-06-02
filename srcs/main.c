/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:48:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 12:56:14 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

void	end_program(void)
{
	rl_clear_history();
	free_split(_data()->path);
}

void	main_help(void)
{
	get_path();
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	*data;

	(void)argc;
	(void)argv;
	(void)envp;
	data = _data();
	main_help();
	while (1)
	{
		input = readline(COLOR_GREEN "minishell >" COLOR_RESET);
		if (!input || ft_strncmp(input, "exit", 5) == 0)
		{
			if (input)
				free(input);
			printf("exit\n");
			break ;
		}
		if (!only_space(input))
			input[0] = '\0';
		if (*input)
			add_history(input);
		if (ft_strncmp(input, "/test", 6) == 0)
			print_tab(data->path);
		printf("Tu as ecrit : %s\n", input);
		free(input);
	}
	end_program();
	return (0);
}
