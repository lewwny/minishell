/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_infix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:38:51 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/23 13:16:37 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast	*infix_pipe(t_ast *left, t_token *op)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		malloc_error();
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

static void	attach_redir_node(t_ast *left, t_ast *new_node)
{
	t_ast	*r;

	r = left;
	while (r->type == AST_REDIR
		&& r->ast.redir.child
		&& r->ast.redir.child->type == AST_REDIR)
		r = r->ast.redir.child;
	if (r->type == AST_REDIR)
	{
		new_node->ast.redir.child = r->ast.redir.child;
		r->ast.redir.child = new_node;
	}
}

static t_ast	*create_redir_node(t_token *op, t_token *file_tok)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (!new_node)
		malloc_error();
	new_node->type = AST_REDIR;
	new_node->ast.redir.type = op->type;
	new_node->ast.redir.target = ft_strdup(file_tok->text);
	new_node->ast.redir.child = NULL;
	return (new_node);
}

static t_ast	*infix_redirection(t_ast *left, t_token *op)
{
	t_token	*file_tok;
	t_ast	*new_node;

	file_tok = advance_token();
	if (!left || !file_tok || file_tok->type != TK_WORD)
	{
		parser_error_at(op, "Expected file after redirection", op->text);
		return (NULL);
	}
	new_node = create_redir_node(op, file_tok);
	if (left->type == AST_REDIR)
	{
		attach_redir_node(left, new_node);
		return (left);
	}
	new_node->ast.redir.child = left;
	return (new_node);
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
