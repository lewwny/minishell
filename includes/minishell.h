/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:49:01 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/13 12:32:06 by macauchy         ###   ########.fr       */
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
# include <stdbool.h>
# include <unistd.h>

# define COLOR_GREEN "\001\033[0;32m\002"
# define COLOR_RESET "\001\033[0m\002"
# define COLOR_RED "\001\033[0;31m\002"

// typedef enum e_redir_type
// {
// 	REDIR_IN,
// 	REDIR_OUT,
// 	REDIR_APPEND,
// 	REDIR_HEREDOC
// }	t_redir_type;

typedef enum e_token_type
{
	TK_WORD,
	TK_PIPE,
	TK_REDIR_IN,
	TK_REDIR_OUT,
	TK_APPEND,
	TK_HEREDOC,
	TK_LPAREN,
	TK_RPAREN,
	TK_SEMICOLON,
	TK_AND,
	TK_OR,
	TK_DUP_IN,
	TK_DUP_OUT,
	TK_BACKGROUND,
	TK_HERESTRING,
	TK_EOF,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	TK_ERROR
}				t_token_type;

typedef struct s_redir
{
	t_token_type	type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	char			*cmd_path;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_ast_type
{
	AST_CMD,
	AST_PIPE,
	AST_REDIR,
	AST_SEQ,
	AST_BG,
	AST_AND,
	AST_OR,
	AST_SUBSHELL
}				t_ast_type;

typedef struct s_token
{
	t_token_type	type;
	char			*text;
	int				left_bp;
	int				right_bp;
}				t_token;

typedef struct s_ast
{
	t_ast_type	type;
	union u_ast
	{
		struct s_ast_cmd
		{
			char		**args;
		} cmd;
		struct s_ast_pipe
		{
			struct s_ast	*left;
			struct s_ast	*right;
		} pipe;
		struct s_ast_redir
		{
			t_token_type	type;
			char			*target;
			struct s_ast	*child;
		} redir;
		struct s_ast_subshell
		{
			struct s_ast	*sub;
		} subshell;
		struct s_ast_logical
		{
			struct s_ast	*left;
			struct s_ast	*right;
		} logical;
	} ast;
}				t_ast;

// typedef struct s_minishell
// {
// 	t_token			*tokens;
// 	char			**cmds;
// 	size_t	pos;
// 	int				exit_status;
// 	bool			early_error;
// 	bool			error;
// 	t_ast			*ast;
// 	t_cmd			*cmd_lst;
// 	size_t			escaped;
// }				t_minishell;

typedef struct s_ctx
{
	char	*arg;
	bool	is_escaped;
}			t_ctx;

typedef struct s_data
{
	t_cmd	*cmds;
	int		exit_code;
	char	**path;
	char	**env;
	char	**args;
	t_ctx	*ctx;
	t_env	*env_list;
	char	*input;
	char	*prompt;
	t_token	*tokens;
	t_ast	*ast;
	size_t	pos;
	int		exit_status;
	bool	early_error;
	bool	error;
	size_t	escaped;
	int		is_last;
	int		fd[2];
	pid_t	pid;
	char	*old_pwd;
}	t_data;

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
char	*get_prompt(void);
void	signal_handler(int sig);
void	ls_builtin(t_cmd *cmd);
char	*replace_env_vars(char *str);
void	handle_child_process(t_cmd *cur, int in_fd, int *fd, int is_last);
void	cleanup_fds(int *in_fd, int *fd, int is_last);
void	free_cmd_path(void);
void	wait_for_children(int *status);
void	exec_single_cmd(t_cmd *cur, int *in_fd, int *fd, int is_last);
void	exec_child(t_cmd *cmd, int in_fd, int out_fd, t_data *data);
void	get_cmd(char *cmd);
char	*get_env_value(const char *key);
void	edit_env(t_env *env_list);
t_env	*create_env_node(char *env);
char	*get_key(char *arg);

// Parsing functions

void	free_ms_ctx(void);
void	free_token_array(void);
char	**split_on_whitespace(char *line);
void	*ft_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void	append_token(unsigned int *cap, unsigned int *c, char *new_tok);
bool	handle_operator(unsigned int *cap, unsigned int *count,
			char *line, unsigned int *i);
bool	handle_word(unsigned int *cap, unsigned int *count,
			char *line, unsigned int *i);
bool	handle_quote(unsigned int *cap, unsigned int *count,
			char *line, unsigned int *i);
void	print_token_array(t_token *tokens);
char	*extract_word(const char *line, unsigned int *i);
char	*extract_operator(const char *line, unsigned int *i);
t_ctx	*collect_quoted(const char *str, char quote, bool *unclosed);
void	finalize_token_array(t_token **tokens, unsigned int *count,
			unsigned int *cap);
void	assign_token_type_and_bp(const char *word, t_token *token);
void	add_token(t_token **arr, unsigned int *count,
			unsigned int *cap, t_token newtok);
void	set_token_fields(t_token *token, t_token_type type, char *text,
			int bp[2]);
void	free_ast(t_ast *ast);
void	free_token_array(void);
void	free_cmdlst(t_cmd *cmd);
t_token	*tokenize_to_pratt(t_ctx *args);
t_token	*advance_token(void);
t_token	*peek_token(void);
t_ast	*parse_prefix(t_token *tok);
t_ast	*parse_infix(t_ast *left, t_token *op);
t_ast	*parse_expression(int min_bp);
void	parser_error_at(t_token *tok, char *msg, char *tk_text);
char	**alloc_args(size_t cap);
char	*dup_arg(char *text);
char	**grow_args(char **args, size_t old_cnt, size_t *cap);
t_cmd	*ast_to_cmd(t_ast *root);
void	append_t_ctx(unsigned int *cap, unsigned int *c, t_ctx *new_tok);

#endif
