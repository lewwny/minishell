/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:51:17 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/20 15:37:36 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_token_fields(t_token *token, t_token_type type, char *text,
		int bp[2])
{
	token->type = type;
	if (text)
	{
		token->text = ft_strdup(text);
		if (!token->text)
			malloc_error();
	}
	else
		token->text = NULL;
	token->left_bp = bp[0];
	token->right_bp = bp[1];
}

void	finalize_token_array(t_token **tokens, unsigned int *count,
		unsigned int *cap)
{
	t_token	tok;

	if (!_data()->error && !_data()->early_error)
	{
		set_token_fields(&tok, TK_EOF, NULL, (int []){0, 0});
		add_token(tokens, count, cap, tok);
	}
	else
	{
		set_token_fields(&tok, TK_ERROR, NULL, (int []){0, 0});
		add_token(tokens, count, cap, tok);
		set_token_fields(&tok, TK_EOF, NULL, (int []){0, 0});
		add_token(tokens, count, cap, tok);
	}
	set_token_fields(&tok, TK_ERROR, NULL, (int []){0, 0});
	add_token(tokens, count, cap, tok);
}

static void	assign_fd_token_type(const char *word, t_token *token,
		unsigned int idx)
{
	if (word[idx] == '>' && word[idx + 1] == '>'
		&& !ft_isspace((unsigned char)word[idx + 2]))
		set_token_fields(token, TK_APPEND, (char *)word, (int []){30, 31});
	else if (word[idx] == '<' && word[idx + 1] == '<'
		&& !ft_isspace((unsigned char)word[idx + 2]))
		set_token_fields(token, TK_HEREDOC, (char *)word, (int []){30, 31});
	else if (word[idx] == '>' && !ft_isspace((unsigned char)word[idx + 1]))
		set_token_fields(token, TK_REDIR_OUT, (char *)word, (int []){30, 31});
	else if (word[idx] == '<' && !ft_isspace((unsigned char)word[idx + 1]))
		set_token_fields(token, TK_REDIR_IN, (char *)word, (int []){30, 31});
	else
		set_token_fields(token, TK_WORD, (char *)word, (int []){0, 0});
}

static void	assign_simple_token_type(const char *word, t_token *token)
{
	if (!ft_strcmp(word, "|"))
		set_token_fields(token, TK_PIPE, (char *)word, (int []){10, 9});
	else if (!ft_strcmp(word, "<"))
		set_token_fields(token, TK_REDIR_IN, (char *)word, (int []){30, 31});
	else if (!ft_strcmp(word, ">"))
		set_token_fields(token, TK_REDIR_OUT, (char *)word, (int []){30, 31});
	else if (!ft_strcmp(word, ">>"))
		set_token_fields(token, TK_APPEND, (char *)word, (int []){30, 31});
	else if (!ft_strcmp(word, "<<"))
		set_token_fields(token, TK_HEREDOC, (char *)word, (int []){30, 31});
	else
		set_token_fields(token, TK_WORD, (char *)word, (int []){0, 0});
}

void	assign_token_type_and_bp(const char *word, t_token *token)
{
	unsigned int	idx;

	idx = 0;
	if (ft_isdigit(word[0]))
	{
		while (ft_isdigit(word[idx]))
			idx++;
		assign_fd_token_type(word, token, idx);
		return ;
	}
	assign_simple_token_type(word, token);
}
