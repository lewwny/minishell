/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_prefix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:01:44 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/23 12:32:00 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**collect_args(t_token *tok)
{
	size_t	cap;
	size_t	count;
	char	**args;
	t_token	*next_tok;

	cap = 4;
	count = 0;
	args = alloc_args(cap);
	args[count++] = dup_arg(tok->text);
	while (peek_token()->type == TK_WORD)
	{
		next_tok = advance_token();
		if (count + 1 >= cap)
			args = grow_args(args, count, &cap);
		args[count++] = dup_arg(next_tok->text);
	}
	args[count] = NULL;
	return (args);
}

static t_ast	*prefix_word(t_token *tok)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		malloc_error();
	node->type = AST_CMD;
	node->ast.cmd.args = collect_args(tok);
	return (node);
}

static t_ast	*create_redir_node(t_token *tok, t_token *file_tok, t_ast *cmd)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
	{
		free_ast(cmd);
		malloc_error();
	}
	node->type = AST_REDIR;
	node->ast.redir.type = tok->type;
	node->ast.redir.target = ft_strdup(file_tok->text);
	if (!node->ast.redir.target)
	{
		free_ast(cmd);
		free(node);
		malloc_error();
	}
	node->ast.redir.child = cmd;
	return (node);
}

static t_ast	*prefix_redir(t_token *tok)
{
	t_token	*file_tok;
	t_ast	*cmd;

	file_tok = advance_token();
	if (!file_tok || file_tok->type != TK_WORD)
	{
		parser_error_at(tok, "Expected file name after redirection", tok->text);
		_data()->early_error = true;
		return (NULL);
	}
	cmd = parse_expression(0);
	if (!cmd)
		return (NULL);
	return (create_redir_node(tok, file_tok, cmd));
}

t_ast	*parse_prefix(t_token *tok)
{
	if (tok->type == TK_WORD)
		return (prefix_word(tok));
	if (tok->type == TK_REDIR_IN
		|| tok->type == TK_REDIR_OUT
		|| tok->type == TK_APPEND
		|| tok->type == TK_HEREDOC)
		return (prefix_redir(tok));
	parser_error_at(tok, "Unexpected token in prefix", tok->text);
	return (NULL);
}
