/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:19:24 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:27:39 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
