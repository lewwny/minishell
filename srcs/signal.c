/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 10:21:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/10 19:38:37 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	sigint_handler(int sig)
{
	(void) sig;
	if (_data()->input)
	{
		free(_data()->input);
		_data()->input = NULL;
	}
	if (_data()->prompt)
	{
		free(_data()->prompt);
		_data()->prompt = NULL;
	}
	rl_on_new_line();
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
	_data()->exit_code = 130;
}

static void	sigquit_handler(int sig)
{
	(void) sig;
	_data()->exit_code = 131;
}

void	signal_handler(int sig)
{
	if (sig == 0)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, sigquit_handler);
	}
}
