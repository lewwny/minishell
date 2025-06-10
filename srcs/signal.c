/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 10:21:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/10 12:10:17 by macauchy         ###   ########.fr       */
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
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	_data()->exit_status = 130;
}

static void	sigquit_handler(int sig)
{
	(void) sig;
	_data()->exit_status = 131;
}

void	signal_handler(int sig)
{
	if (sig == 0)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, sigquit_handler);
	}
}
