/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:47:36 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/20 15:35:08 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_cmd_in_path(char *cmd, t_data *data)
{
	int	i;

	i = 0;
	while (data->path[i])
	{
		if (data->cmds->cmd_path)
			free(data->cmds->cmd_path);
		data->cmds->cmd_path = ft_strjoin(data->path[i], cmd);
		if (!data->cmds->cmd_path)
			malloc_error();
		if (access(data->cmds->cmd_path, X_OK) == 0)
			return (1);
		free(data->cmds->cmd_path);
		data->cmds->cmd_path = NULL;
		i++;
	}
	return (0);
}

void	get_cmd(char *cmd)
{
	t_data	*data;

	data = _data();
	if (access(cmd, X_OK) == 0)
	{
		data->cmds->cmd_path = ft_strdup(cmd);
		if (!data->cmds->cmd_path)
			malloc_error();
		return ;
	}
	if (!data->path)
	{
		data->cmds->cmd_path = NULL;
		return ;
	}
	if (!find_cmd_in_path(cmd, data))
		data->cmds->cmd_path = NULL;
}
