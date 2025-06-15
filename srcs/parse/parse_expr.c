/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:28:28 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/15 14:37:31 by lengarci         ###   ########.fr       */
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

t_ast	*parse_expression(int min_bp)
{
	t_token		*token;
	t_ast		*left;
	t_token		*op;
	t_data		*minishell;

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
