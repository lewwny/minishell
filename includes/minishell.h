/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:49:01 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/02 12:43:58 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <termcap.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>

# define COLOR_GREEN "\001\033[0;32m\002"
# define COLOR_RESET "\001\033[0m\002"

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_data
{
	t_cmd	*cmds;
	int		exit_code;
	char	**path;
}	t_data;

extern int	g_exit_status;

int		only_space(char *str);
void	free_split(char **tab);
void	malloc_error(void);
void	print_tab(char **tab);
void	get_path(void);
t_data	*_data(void);

#endif
