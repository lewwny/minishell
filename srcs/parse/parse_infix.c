/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_infix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:38:51 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/19 13:19:15 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_redir_target(t_token *file_tok, t_token *op)
{
	char	*target;

	if (!file_tok || file_tok->type != TK_WORD)
	{
		parser_error_at(op, "Expected file name after redirection", op->text);
		return (NULL);
	}
	target = ft_strdup(file_tok->text);
	if (!target)
	{
		dprintf(2, "Error: Memory allocation failed for redirection target\n");
		exit(EXIT_FAILURE);
	}
	return (target);
}

static t_ast	*infix_pipe(t_ast *left, t_token *op)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
	{
		dprintf(2, "Error: Memory allocation failed for AST node\n");
		exit(EXIT_FAILURE);
	}
	node->type = AST_PIPE;
	node->ast.pipe.left = left;
	node->ast.pipe.right = parse_expression(op->right_bp);
	if (!node->ast.pipe.right)
	{
		free(node);
		parser_error_at(op, "Expected right side of pipe", op->text);
		return (NULL);
	}
	return (node);
}

static t_ast	*infix_redirection(t_ast *left, t_token *op)
{
	t_token	*file_tok;
	t_ast	*node;

	file_tok = advance_token();
	node = malloc(sizeof(t_ast));
	if (!node)
	{
		dprintf(2, "Error: Memory allocation failed for AST node\n");
		exit(EXIT_FAILURE);
	}
	node->type = AST_REDIR;
	node->ast.redir.type = op->type;
	node->ast.redir.target = get_redir_target(file_tok, op);
	if (!node->ast.redir.target)
	{
		free(node);
		return (NULL);
	}
	node->ast.redir.child = left;
	return (node);
}

t_ast	*parse_infix(t_ast *left, t_token *op)
{
	if (op->type == TK_PIPE)
		return (infix_pipe(left, op));
	if (op->type == TK_REDIR_IN || op->type == TK_REDIR_OUT
		|| op->type == TK_APPEND || op->type == TK_HEREDOC
		|| op->type == TK_DUP_IN || op->type == TK_DUP_OUT
		|| op->type == TK_AND || op->type == TK_OR)
		return (infix_redirection(left, op));
	parser_error_at(op, "Unexpected token in infix", op->text);
	return (NULL);
}
