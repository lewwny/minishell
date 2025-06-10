/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:28:28 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/10 14:53:08 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	if (tok->type == TK_ERROR)
		return ;
	dprintf(2, "Error: %s at token '%s' (type: %d)\n", msg, tk_text, tok->type);
	_data()->early_error = true;
	if (tok->type == TK_EOF)
		_data()->error = true;
	else
		_data()->error = true;
}

t_ast	*parse_expression(int min_bp)
{
	t_token		*token;
	t_ast		*left;
	t_token		*op;
	t_data	*minishell;

	minishell = _data();
	if (minishell->early_error || minishell->error)
		return (NULL);
	token = advance_token();
	left = parse_prefix(token);
	if (!left || minishell->error || minishell->early_error)
		return (free_ast(left), NULL);
	while (!minishell->error && peek_token()->left_bp > min_bp)
	{
		op = advance_token();
		left = parse_infix(left, op);
		if (!left || minishell->error || minishell->early_error)
		{
			if (op->type == TK_ERROR)
				parser_error_at(op, "Unexpected token", op->text);
			return (free_ast(left), NULL);
		}
	}
	return (left);
}
