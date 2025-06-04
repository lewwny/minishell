/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:49:01 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/04 10:56:40 by lengarci         ###   ########.fr       */
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
	char			*cmd_path;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	t_cmd	*cmds;
	int		exit_code;
	char	**path;
	char	**env;
	t_env	*env_list;
	char	*input;
}	t_data;

extern int	g_exit_status;

int		only_space(char *str);
void	free_split(char **tab);
void	malloc_error(void);
void	print_tab(char **tab);
void	get_path(void);
t_data	*_data(void);
t_cmd	*ft_cmdnew(char **content);
void	parsing(char *input);
void	free_cmd(void);
void	exec_cmds(t_cmd *cmd);
void	exec_builtins(t_cmd *cmd);
int		is_builtin(char *cmd);
void	end_program(void);
t_env	*env_fill(char **envp);
void	print_env_list(t_env *env);
void	delete_env_node(t_env **env, const char *key);
char	**env_to_array(t_env *env);
void	env_builtin(void);
void	exit_builtin(void);
void	pwd_builtin(void);
void	cd_builtin(void);
void	unset_builtin(void);
void	export_builtin(void);
void	lst_add_back_env(t_env **env, t_env *new_node);
void	ultimate_free_func(void);
void	free_split(char **tab);
void	ft_cmdclear(t_cmd **cmd);

#endif
