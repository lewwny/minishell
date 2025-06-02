/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:36:22 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 12:46:07 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_data	*_data(void)
{
	static t_data	data;
	static int		init = 0;

	if (init == 0)
	{
		init = 1;
		ft_bzero(&data, sizeof(t_data));
	}
	return (&data);
}
