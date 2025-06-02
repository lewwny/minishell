/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:48:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 08:52:45 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

int	main(int argc, char **argv, char **envp)
{
	char	*input;

	(void)argc;
	(void)argv;
	(void)envp;
	while (1)
	{
		input = readline(COLOR_GREEN "minishell >" COLOR_RESET);
		if (!input || strcmp(input, "exit") == 0)
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
		printf("Tu as ecrit : %s\n", input);
		free(input);
	}
	rl_clear_history();
	return (0);
}
