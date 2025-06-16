/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:19:24 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/16 09:43:20 by lengarci         ###   ########.fr       */
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
		return (exit_code);
	}
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			write(2, "minishell: exit: numeric argument required\n", 44);
			return (exit_code);
		}
		i++;
	}
	return (ft_atoi(arg));
}

void	exit_builtin(void)
{
	if (_data()->cmds->args[1] == NULL)
		_data()->exit_code = 0;
	else
	{
		_data()->exit_code = verify_exit_code(_data()->cmds->args[1]);
		if (_data()->cmds->args[2])
		{
			write(2, "minishell: exit: too many arguments\n", 37);
			_data()->exit_code = 1;
		}
	}
	ultimate_free_func();
	exit(_data()->exit_code);
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
