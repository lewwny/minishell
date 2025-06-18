/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:11:46 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 14:28:01 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	copy_path(char *fullpath, size_t size, const char *path)
{
	int	i;
	int	len_path;

	i = 0;
	len_path = 0;
	while (path[len_path])
		len_path++;
	while (i < len_path && i < (int)(size - 1))
	{
		fullpath[i] = path[i];
		i++;
	}
	if (i > 0 && fullpath[i - 1] != '/' && i < (int)(size - 1))
	{
		fullpath[i] = '/';
		i++;
	}
	return (i);
}

static void	build_fullpath(char *fullpath, size_t size, const char *path,
					const char *name)
{
	int	i;
	int	j;
	int	len_name;

	j = 0;
	len_name = 0;
	i = copy_path(fullpath, size, path);
	while (name[len_name])
		len_name++;
	while (j < len_name && i < (int)(size - 1))
	{
		fullpath[i] = name[j];
		i++;
		j++;
	}
	fullpath[i] = '\0';
}

static void	print_entry(const char *name, struct stat *st, int *first)
{
	if (!*first)
		printf(" ");
	*first = 0;
	if (S_ISDIR(st->st_mode))
		printf("\033[1;34m%s\033[0m", name);
	else if (st->st_mode & S_IXUSR)
		printf("\033[1;32m%s\033[0m", name);
	else
		printf("%s", name);
}

static void	ls_utils(const char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	struct stat		st;
	char			fullpath[1024];
	int				first;

	dir = opendir(path);
	first = 1;
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue ;
		}
		build_fullpath(fullpath, sizeof(fullpath), path, entry->d_name);
		if (stat(fullpath, &st) == 0)
			print_entry(entry->d_name, &st, &first);
		entry = readdir(dir);
	}
	printf("\n");
	closedir(dir);
}

void	ls_builtin(t_cmd *cmd)
{
	if (cmd->args[1])
		ls_utils(cmd->args[1]);
	else
		ls_utils(".");
}
