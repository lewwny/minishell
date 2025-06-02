/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:48:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 08:27:13 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_xit_status = 0;

int	main(int argc, char **argv, char **envp)
{
	char	*input;

	(void)argc;
	(void)argv;
	(void)envp;
	while (1)
	{
		input = readline("minishell >");
		if (!input || strcmp(input, "exit") == 0)
		{
			if (input)
				free(input);
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		printf("Tu as ecrit : %s\n", input);
		free(input);
	}
	rl_clear_history();
	return (0);
}
