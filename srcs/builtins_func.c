/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:19:24 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/12 12:20:24 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_builtin(void)
{
	print_env_list(_data()->env_list);
}

void	exit_builtin(void)
{
	ultimate_free_func();
	exit(0);
}

void	pwd_builtin(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
}

void	cd_builtin(void)
{
	char	*path;

	if (_data()->cmds->args[1] == NULL)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_dprintf(2, "cd: HOME not set\n");
			_data()->exit_code = 1;
			return ;
		}
	}
	else
	{
		path = _data()->cmds->args[1];
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		_data()->exit_code = 1;
	}
}

void	unset_builtin(void)
{
	if (_data()->cmds->args[1] == NULL)
	{
		fprintf(stderr, "unset: not enough arguments\n");
		return ;
	}
	delete_env_node(&_data()->env_list, _data()->cmds->args[1]);
	free_split(_data()->env);
	_data()->env = env_to_array(_data()->env_list);
}
