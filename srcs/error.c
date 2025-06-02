/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:33:22 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 12:43:44 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	malloc_error(void)
{
	rl_clear_history();
	if (_data()->path)
		free_split(_data()->path);
	perror("malloc");
	exit(1);
}
