/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:48:33 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/11 08:33:35 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_ms_ctx(void)
{
	t_data	*data;

	data = _data();
	free_token_array();
	data->error = false;
	data->early_error = false;
	data->pos = 0;
	data->escaped = 0;
}

void	free_token_array(void)
{
	t_data	*data;
	size_t		i;

	data = _data();
	i = 0;
	if (!data->tokens)
		return ;
	while (data->tokens[i].text)
	{
		free(data->tokens[i].text);
		i++;
	}
	free(data->tokens);
}
