/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_prefix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:01:44 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/11 18:46:51 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_ast	*prefix_word(t_token *tok)
{
	t_ast	*node;
	size_t	cap = 4;
	size_t	count = 0;
	char	**args;
	t_token	*next_tok;

	node = malloc(sizeof(t_ast));
	if (!node)
	{
		dprintf(2, "Error: Memory allocation failed for AST node\n");
		exit(EXIT_FAILURE);
	}
	node->type = AST_CMD;
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
	node->ast.cmd.args = args;
	return (node);
}

static t_ast	*prefix_redir(t_token *tok)
{
	t_token	*file_tok;
	t_ast	*cmd;
	t_ast	*node;

	file_tok = advance_token();
	if (!file_tok || file_tok->type != TK_WORD)
	{
		parser_error_at(tok, "Expected file name after redirection", tok->text);
		// free_split(_data()->args);
		return (NULL);
	}
	cmd = parse_expression(0);
	if (!cmd)
		return (NULL);
	node = malloc(sizeof(t_ast));
	node->type = AST_REDIR;
	node->ast.redir.type = tok->type;
	node->ast.redir.target = ft_strdup(file_tok->text);
	if (!node->ast.redir.target)
	{
		dprintf(2, "Error: Memory allocation failed for redirection target\n");
		free_ast(cmd);
		free(node);
		exit(EXIT_FAILURE);
	}
	node->ast.redir.child = cmd;
	return (node);
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
