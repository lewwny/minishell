/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:28:28 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/17 18:42:21 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*advance_token(void)
{
	t_data	*minishell;

	minishell = _data();
	return (&minishell->tokens[minishell->pos++]);
}

t_token	*peek_token(void)
{
	t_data	*minishell;

	minishell = _data();
	return (&minishell->tokens[minishell->pos]);
}

void	parser_error_at(t_token *tok, char *msg, char *tk_text)
{
	(void)tok;
	(void)msg;
	(void)tk_text;
	_data()->early_error = true;
	_data()->error = true;
	_data()->exit_code = 2;
	if (tok->type == TK_ERROR)
		return ;
	if (!msg)
	{
		if (tk_text)
			ft_dprintf(STDERR_FILENO,
				"minishell: %s: %s\n", tk_text, "syntax error");
		else
			ft_dprintf(STDERR_FILENO, "minishell: syntax error\n");
	}
	else
	{
		if (tk_text)
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", tk_text, msg);
		else
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", msg, tok->text);
	}
}

// t_ast *_parse_infix_loop(t_ast *root, int min_bp)
// {
// 	t_data	*ms = _data();
// 	t_ast	*node = root;

// 	while (!ms->error && peek_token()->left_bp > min_bp)
// 	{
// 		t_token *op   = advance_token();
// 		t_ast   *next = parse_infix(node, op);
// 		if (!next || ms->error || ms->early_error)
// 		{
// 			free_ast(root);
// 			if (op->type == TK_ERROR)
// 				parser_error_at(op, "Unexpected token", op->text);
// 			return NULL;
// 		}
// 		node = next;
// 	}
// 	return node;
// }

// t_ast *_parse_prefix_root(void)
// {
// 	t_data	*ms = _data();
// 	t_token	*token;
// 	t_ast	*root;

// 	token = advance_token();
// 	root = parse_prefix(token);
// 	if (!root || ms->error || ms->early_error)
// 	{
// 		free_ast(root);
// 		return NULL;
// 	}
// 	return root;
// }


t_ast	*parse_expression(int min_bp)
{
	t_data	*ms;
	t_token	*token;
	t_ast	*root;
	t_ast	*node;

	ms = _data();
	ms->ctx->sp = 0;
	if (ms->early_error || ms->error)
		return NULL;
	token = advance_token();
	root  = parse_prefix(token);
	if (!root || ms->error || ms->early_error)
	{
		free_ast(root);
		return NULL;
	}
	node = root;
	while (!ms->error && peek_token()->left_bp > min_bp)
	{
		t_token *op   = advance_token();
		t_ast   *next = parse_infix(node, op);
		if (!next || ms->error || ms->early_error)
		{
			free_ast(root);
			if (op->type == TK_ERROR)
				parser_error_at(op, "Unexpected token", op->text);
			return NULL;
		}
		node = next;
	}
	return (node);
}
