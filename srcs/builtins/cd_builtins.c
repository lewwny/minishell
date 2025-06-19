/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:25:13 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/19 11:24:04 by macauchy         ###   ########.fr       */
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
			return (NULL);
		}
	}
	else
	{
		path = _data()->cmds->args[1];
		if (_data()->cmds->args[2])
		{
			ft_dprintf(2, "cd: too many arguments\n");
			return (NULL);
		}
	}
	return (path);
}

static int	change_directory(const char *path, char *oldpwd)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		_data()->exit_code = 1;
		free(oldpwd);
		return (1);
	}
	change_oldpwd(oldpwd);
	free(oldpwd);
	_data()->exit_code = 0;
	return (0);
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
		_data()->exit_code = 1;
		return ;
	}
	change_directory(path, oldpwd);
}
