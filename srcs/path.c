/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:24:29 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 15:45:21 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	put_slash(char **path)
{
	int	i;
	int	len;

	i = 0;
	if (!path || !*path)
		return ;
	while (path[i])
	{
		len = ft_strlen(path[i]);
		path[i][len] = '/';
		path[i][len + 1] = '\0';
		i++;
	}
}

void	get_path(void)
{
	t_data	*data;

	data = _data();
	data->path = ft_split(getenv("PATH"), ':');
	if (!data->path)
		malloc_error();
	put_slash(data->path);
}
