/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:05:59 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/03 17:14:23 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

static void	env_builtin(void)
{
	print_env_list(_data()->env_list);
	printf("==========================\nDEBUG: env_builtin called\n");
}

static void	exit_builtin(void)
{
	free_cmd();
	end_program();
	printf("==========================\nDEBUG: exit_builtin called\n");
	exit(0);
}

static void	pwd_builtin(void)
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

static void	cd_builtin(void)
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

static void unset_builtin(void)
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

void	exec_builtins(t_cmd *cmd)
{
	if (ft_strcmp(cmd->args[0], "env") == 0)
		env_builtin();
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		exit_builtin();
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		pwd_builtin();
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		cd_builtin();
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		unset_builtin();
	// else if (ft_strcmp(cmd->args[0], "echo") == 0)
	// 	echo_builtin(cmd);
	// else if (ft_strcmp(cmd->args[0], "export") == 0)
	// 	export_builtin(cmd);

}
