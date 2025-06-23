/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 07:49:01 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 14:50:00 by lengarci         ###   ########.fr       */
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
# include <errno.h>

# define COLOR_GREEN "\001\033[0;32m\002"
# define COLOR_RESET "\001\033[0m\002"
# define COLOR_RED "\001\033[0;31m\002"

extern int	g_signal_status;

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
	int				heredoc_fd;
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

typedef struct s_ctx
{
	char			*arg;
	bool			is_escaped;
	bool			unclosed;
	unsigned int	len;
	unsigned int	cap;
	unsigned int	sp;
	t_ast			*next;
}					t_ctx;

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
	bool	has_escaped;
	int		flag;
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
void	ft_cmdclear(t_cmd **cmd);
char	*get_prompt(void);
void	signal_handler(int sig);
void	ls_builtin(t_cmd *cmd);
char	*replace_env_vars(char *str);
void	handle_child_process(t_cmd *cur, int in_fd, int *fd, int is_last);
void	cleanup_fds(int *in_fd, int *fd, int is_last);
void	free_cmd_path(void);
void	wait_for_children(int *status);
int		exec_single_cmd(t_cmd *cur, int *in_fd, int *fd, int is_last);
void	exec_child(t_cmd *cmd, int in_fd, int out_fd, t_data *data);
void	get_cmd(char *cmd);
char	*get_env_value(const char *key);
void	edit_env(void);
t_env	*create_env_node(char *env);
char	*get_key(char *arg);
void	change_oldpwd(char *oldpwd);
int		apply_redir(t_redir *r);
void	here_doc_manage(t_redir *redir);
void	print_export(t_env *env);
int		in_env(char *key);
void	edit_value(char *key, char *value);
int		get_heredoc_fd(const char *limiter, int *tmp);
int		check_all_files(t_redir *redirs);
void	split_env(const char *env, char **key, char **value);

// Parsing functions

size_t	find_closing_quote(const char *str, char quote);
void	free_ms_ctx(void);
void	free_ctx_str(void);
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
t_ctx	collect_quoted(const char *str, char quote, bool *unclosed);
void	finalize_token_array(t_token **tokens, unsigned int *count,
			unsigned int *cap);
void	assign_token_type_and_bp(const char *word, t_token *token);
void	add_token(t_token **arr, unsigned int *count,
			unsigned int *cap, t_token newtok);
void	set_token_fields(t_token *token, t_token_type type, char *text,
			int bp[2]);
void	free_ast(t_ast *ast);
void	free_redir(t_redir *redir);
void	free_cmdlst(t_cmd *cmd);
t_token	*tokenize_to_pratt(t_ctx *args);
t_token	*advance_token(void);
t_token	*peek_token(void);
t_ast	*parse_prefix(t_token *tok);
t_ast	*parse_infix(t_ast *left, t_token *op);
t_ast	*parse_expression(int min_bp);
void	parser_error_at(t_token *tok, char *msg, char *tk_text);
t_ctx	collect_word_ctx(const char *line, unsigned int *i);
char	**alloc_args(size_t cap);
char	*dup_arg(char *text);
char	**grow_args(char **args, size_t old_cnt, size_t *cap);
t_cmd	*ast_to_cmd(t_ast *root);
void	append_t_ctx(unsigned int *cap, unsigned int *c, t_ctx *new_tok);
t_cmd	*new_cmd(void);
char	**copy_args(char **src);
char	**get_args_from_ctx(t_ctx *ctx);
t_ast	*gather_redirs(t_ast *node, t_redir **rlist);
t_cmd	*flatten_pipe(t_ast *root);
bool	collect_quoted_ctx(t_ctx *ctx, const char *str, char quote,
			unsigned int *i);
bool	ensure_capacity(char **buf, unsigned int *cap, unsigned int len);
t_ctx	*collect_word_fragments(const char *line, unsigned int *i,
			size_t *count);
void	skip_whitespace(const char *line, unsigned int *i);
void	join_fragments_env(t_ctx *frag);
t_ctx	join_fragments(t_ctx *frags, size_t count);
bool	process_token(unsigned int *cap, unsigned int *count,
			char *line, unsigned int *i);
bool	add_fragment(t_ctx **fragments, size_t *idx, size_t *cap,
			t_ctx *frag);
bool	collect_one_fragment(const char *line, unsigned int *i, t_ctx *frag);
t_ctx	*alloc_fragments(size_t cap);
bool	ensure_str_capacity(char **buf, unsigned int *cap, unsigned int needed);
bool	init_fragment(t_ctx *frag);
void	handle_double_quote_escape(const char *str, unsigned int *j,
			unsigned int *p, t_ctx *buff);
void	handle_single_quote_dollar(unsigned int *j, unsigned int *p,
			t_ctx *buff);
void	redir_add_back(t_redir **lst, t_redir *new);

#endif
