/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:25:13 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:26:38 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_cd_path(void)
{
	char	*path;

	if (_data()->cmds->args[1] == NULL)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_dprintf(2, "cd: HOME not set\n");
			_data()->exit_code = 1;
			return (NULL);
		}
	}
	else
	{
		path = _data()->cmds->args[1];
	}
	return (path);
}

void	cd_builtin(void)
{
	char	*path;
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("cd");
		_data()->exit_code = 1;
		return ;
	}
	path = get_cd_path();
	if (!path)
	{
		free(oldpwd);
		return ;
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		_data()->exit_code = 1;
	}
	else
		change_oldpwd(oldpwd);
	free(oldpwd);
}
