/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:28:28 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/19 13:19:15 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static t_ast	*parse_expression_loop(t_ast *root, int min_bp)
{
	t_data	*ms;
	t_ast	*node;
	t_token	*op;
	t_ast	*next;

	ms = _data();
	node = root;
	while (!ms->error && peek_token()->left_bp > min_bp)
	{
		op = advance_token();
		next = parse_infix(node, op);
		if (!next || ms->error || ms->early_error)
		{
			free_ast(root);
			if (op->type == TK_ERROR)
				parser_error_at(op, "Unexpected token", op->text);
			return (NULL);
		}
		node = next;
	}
	return (node);
}

static t_ast	*parse_expression_start(void)
{
	t_data	*ms;
	t_token	*token;
	t_ast	*root;

	ms = _data();
	ms->ctx->sp = 0;
	if (ms->early_error || ms->error)
		return (NULL);
	token = advance_token();
	root = parse_prefix(token);
	if (!root || ms->error || ms->early_error)
	{
		free_ast(root);
		return (NULL);
	}
	return (root);
}

t_ast	*parse_expression(int min_bp)
{
	t_ast	*root;

	root = parse_expression_start();
	if (!root)
		return (NULL);
	return (parse_expression_loop(root, min_bp));
}
