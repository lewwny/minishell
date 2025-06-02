/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 08:40:27 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 08:53:02 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	only_space(char *str)
{
	int	i;
	int	letter;

	letter = 0;
	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if ((str[i] != 32) && (str[i] < 9 || str[i] > 13))
			letter++;
		i++;
	}
	return (letter);
}
