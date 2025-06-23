/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:08:36 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/23 13:39:49 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_token_array(void)
{
	t_data		*ms;
	size_t		i;

	ms = _data();
	i = 0;
	if (!ms->tokens)
		return ;
	while (ms->tokens[i].text)
	{
		free(ms->tokens[i].text);
		i++;
	}
	free(ms->tokens);
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == AST_CMD)
	{
		free_split(ast->ast.cmd.args);
	}
	else if (ast->type == AST_PIPE)
	{
		free_ast(ast->ast.pipe.left);
		free_ast(ast->ast.pipe.right);
	}
	else if (ast->type == AST_REDIR)
	{
		free(ast->ast.redir.target);
		free_ast(ast->ast.redir.child);
	}
	else if (ast->type == AST_SUBSHELL)
	{
		free_ast(ast->ast.subshell.sub);
	}
	free(ast);
}

char	**grow_args(char **args, size_t old_cnt, size_t *cap)
{
	*cap *= 2;
	args = ft_realloc(args, sizeof(char *) * old_cnt, sizeof(char *) * (*cap));
	if (!args)
	{
		ft_dprintf(2, "Error: Memory allocation failed for command args\n");
		exit(EXIT_FAILURE);
	}
	return (args);
}

char	**alloc_args(size_t cap)
{
	char	**args;

	args = malloc(sizeof(char *) * cap);
	if (!args)
		malloc_error();
	return (args);
}

char	*dup_arg(char *text)
{
	char	*arg;

	arg = ft_strdup(text);
	if (!arg)
		malloc_error();
	return (arg);
}
