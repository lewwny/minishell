/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:26:00 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/12 16:45:55 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_token(t_token **arr, unsigned int *count,
		unsigned int *cap, t_token newtok)
{
	if (*count + 1 >= *cap)
	{
		if (*cap == 0)
			*cap = 8;
		else
			*cap = *cap * 2;
		*arr = ft_realloc(*arr, sizeof(t_token) * (*count),
				sizeof(t_token) * (*cap));
		if (!*arr)
		{
			perror("minishell: realloc");
			exit(EXIT_FAILURE);
		}
	}
	(*arr)[*count] = newtok;
	(*count)++;
}

int	process_regular_token(char **args, int i, t_token **tokens,
			unsigned int *count, unsigned int *cap)
{
	t_token	new_token;
	char	*word;
	char	*ctx_word;

	word = args[i];
	ctx_word = _data()->ctx[i].arg;
	assign_token_type_and_bp(ctx_word, &new_token);
	add_token(tokens, count, cap, new_token);
	return (1);
}

static void	handle_null_cmds(t_token **tokens, unsigned int *count,
		unsigned int *cap)
{
	t_token	null_tok;

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
	while (_data()->ctx[i].arg && !_data()->error)
	{
		step = process_regular_token(args, i, tokens, count, cap);
		i += step;
	}
}

char	**get_args_from_ctx(t_ctx *ctx)
{
	char	**args;
	size_t	i;
	size_t	size;

	size = 0;
	if (!ctx || !ctx->arg)
		return (NULL);
	while (_data()->args[size])
		size++;
	args = malloc(sizeof(char *) * (size + 1));
	if (!args)
	{
		perror("minishell: calloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < size && ctx[i].arg)
	{
		args[i] = ft_strdup(ctx[i].arg);
		if (!args[i])
		{
			perror("minishell: strdup");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	args[i] = NULL;
	return (args);
}

//change char **args to t_ctx *ctx;
t_token	*tokenize_to_pratt(t_ctx *args)
{
	unsigned int	cap;
	unsigned int	count;
	t_token			*tokens;
	char			**args_array;

	cap = 0;
	count = 0;
	tokens = NULL;
	_data()->early_error = false;
	if (!args || !_data()->ctx)
	{
		handle_null_cmds(&tokens, &count, &cap);
		return (tokens);
	}
	args_array = get_args_from_ctx(args);
	process_args(args_array, &tokens, &count, &cap);
	free_split(args_array);
	finalize_token_array(&tokens, &count, &cap);
	return (tokens);
}
