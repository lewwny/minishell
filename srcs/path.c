/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:24:29 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 12:47:02 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_path(void)
{
	t_data	*data;

	data = _data();
	data->path = ft_split(getenv("PATH"), ':');
	if (!data->path)
		malloc_error();
}
