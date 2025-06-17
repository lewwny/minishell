/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 10:21:15 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/17 18:43:31 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	_data()->exit_code = 130;
	rl_on_new_line();
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	heredoc_sigint_handler(int sig)
{
	(void) sig;
	close(0);
	open("/dev/stdin", O_RDONLY);
	_data()->exit_code = 130;
}

static void	sigquit_handler(int sig)
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
	_data()->exit_code = 131;
	write(2, "Quit: 3\n", 9);
	rl_on_new_line();
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signal_handler(int sig)
{
	if (sig == 0)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 1)
	{
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 2)
	{
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, sigquit_handler);
	}
}
