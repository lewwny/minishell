/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:19:24 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 13:39:39 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	env_builtin(void)
{
	print_env_list(_data()->env_list);
}

static int	verify_exit_code(char *arg)
{
	int	exit_code;
	int	i;

	exit_code = 2;
	i = 0;
	if (arg[0] == '\0')
	{
		write(2, "minishell: exit: numeric argument required\n", 44);
		ultimate_free_func();
		exit(exit_code);
	}
	if ((arg[0] == '-' || arg[0] == '+') && arg[1] != '\0')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			write(2, "minishell: exit: numeric argument required\n", 44);
			ultimate_free_func();
			exit(exit_code);
		}
		i++;
	}
	return (ft_atoi(arg));
}

void	exit_builtin(void)
{
	if (_data()->cmds->args[1] == NULL)
		_data()->exit_status = 0;
	else
	{
		_data()->exit_status = verify_exit_code(_data()->cmds->args[1]);
		if (_data()->cmds->args[2])
		{
			write(2, "minishell: exit: too many arguments\n", 37);
			_data()->exit_status = 1;
		}
	}
	if (_data()->exit_status == 1)
	{
		_data()->exit_code = 1;
		return ;
	}
	ultimate_free_func();
	exit(_data()->exit_status);
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
	int	i;

	i = 1;
	if (_data()->cmds->args[1] == NULL)
	{
		write(2, "unset: not enough arguments\n", 29);
		return ;
	}
	while (_data()->cmds->args[i])
	{
		delete_env_node(&_data()->env_list, _data()->cmds->args[i]);
		free_split(_data()->env);
		_data()->env = env_to_array(_data()->env_list);
		i++;
	}
}
