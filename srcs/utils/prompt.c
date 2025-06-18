/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 09:07:39 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:35:03 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_prompt1(char *cwd)
{
	char	*dir;
	char	*colored_dir;
	char	*prompt;
	char	*tmp;

	dir = ft_strrchr(cwd, '/');
	if (dir && dir[1])
		dir++;
	else
		dir = cwd;
	if (_data()->exit_code != 0)
		colored_dir = ft_strjoin(COLOR_RED, dir);
	else
		colored_dir = ft_strjoin(COLOR_GREEN, dir);
	if (!colored_dir)
		malloc_error();
	tmp = ft_strjoin(colored_dir, "\033[0m $ ");
	free(colored_dir);
	if (!tmp)
		malloc_error();
	prompt = ft_strdup(tmp);
	free(tmp);
	if (!prompt)
		malloc_error();
	return (prompt);
}

static char	*get_prompt2(char *cwd)
{
	char	*colored_dir;
	char	*prompt;
	char	*tmp;

	colored_dir = ft_strjoin(COLOR_GREEN, cwd);
	if (_data()->exit_code != 0)
		colored_dir = ft_strjoin(COLOR_RED, cwd);
	if (!colored_dir)
		malloc_error();
	tmp = ft_strjoin(colored_dir, "\033[0m $ ");
	free(colored_dir);
	if (!tmp)
		malloc_error();
	prompt = ft_strdup(tmp);
	free(tmp);
	if (!prompt)
		malloc_error();
	return (prompt);
}

char	*get_prompt(void)
{
	char	*prompt;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		cwd = ft_strdup("unknown_directory");
		if (!cwd)
			malloc_error();
	}
	if (!cwd[1])
	{
		prompt = get_prompt2(cwd);
	}
	else
		prompt = get_prompt1(cwd);
	free(cwd);
	return (prompt);
}
