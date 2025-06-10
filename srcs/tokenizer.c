/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:26:00 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/10 12:32:41 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_null_cmds(t_token **tokens, unsigned int *count,
		unsigned int *cap)
{
	t_token null_tok;

	set_token_fields(&null_tok, TK_ERROR, NULL, (int []){0, 0});
	add_token(tokens, count, cap, null_tok);
}

static void	process_args(char **args, t_token **tokens, unsigned int *count,
		unsigned int *cap)
{
	size_t	i;
	size_t	step;

	i = 0;
	step = 0;
	while (args[i] && !_data()->error)
	{
		if (is_fd_redirection(args, i))
		{
			step = process_fd_redirection(args, i, tokens, count, cap);
			i += step;
			continue ;
		}
		step = process_regular_token(args, i, tokens, count, cap);
		i += step;
	}
}

t_token	*tokenize_to_pratt(char **args)
{
	unsigned int	cap;
	unsigned int	count;
	t_token			*tokens;

	cap = 0;
	count = 0;
	tokens = NULL;
	_data()->early_error = false;
	if (!args)
	{
		handle_null_cmds(&tokens, &count, &cap);
		return (tokens);
	}
	process_args(args, &tokens, &count, &cap);
	finalize_token_array(&tokens, &count, &cap);
	print_token_array(tokens);
	return (tokens);
}
