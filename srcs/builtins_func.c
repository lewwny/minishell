/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:19:24 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/04 11:51:48 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_builtin(void)
{
	print_env_list(_data()->env_list);
	printf("==========================\nDEBUG: env_builtin called\n");
}

void	exit_builtin(void)
{
	ultimate_free_func();
	printf("==========================\nDEBUG: exit_builtin called\n");
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
	printf("==========================\nDEBUG: pwd_builtin called\n");
}

void	cd_builtin(void)
{
	char	*path;

	if (_data()->cmds->args[1] == NULL)
	{
		path = getenv("HOME");
		if (!path)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return ;
		}
	}
	else
	{
		path = _data()->cmds->args[1];
	}
	if (chdir(path) != 0)
		perror("cd");
	printf("==========================\nDEBUG: cd_builtin called\n");
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
	printf("==========================\nDEBUG: unset_builtin called\n");
}
