/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_to_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:58:57 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/23 12:07:36 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*flatten_pipe_on_redir(t_ast *leaf, t_redir *redir_list)
{
	t_cmd	*pipe_list;
	t_redir	*last;

	last = NULL;
	pipe_list = flatten_pipe(leaf);
	if (!pipe_list)
	{
		free_redir(redir_list);
		return (NULL);
	}
	if (redir_list)
	{
		last = redir_list;
		while (last->next)
			last = last->next;
		last->next = pipe_list->redirs;
		pipe_list->redirs = redir_list;
	}
	return (pipe_list);
}

static t_cmd	*handle_cmd_leaf(t_ast *leaf, t_redir *redir_lst)
{
	t_cmd	*cmd;

	cmd = new_cmd();
	cmd->redirs = redir_lst;
	if (leaf->type == AST_CMD)
		cmd->args = copy_args(leaf->ast.cmd.args);
	else
	{
		cmd->args = (char **)malloc(sizeof(char *));
		if (!cmd->args)
			malloc_error();
	}
	return (cmd);
}

void	redir_add_back(t_redir **lst, t_redir *new)
{
	t_redir	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = new;
	}
	new->next = NULL;
}

t_cmd	*switch_type(t_ast *root)
{
	t_redir	*redir_lst;
	t_ast	*leaf;

	redir_lst = NULL;
	leaf = gather_redirs(root, &redir_lst);
	if (!leaf)
	{
		free_redir(redir_lst);
		return (NULL);
	}
	if (leaf->type == AST_PIPE)
		return (flatten_pipe_on_redir(leaf, redir_lst));
	return (handle_cmd_leaf(leaf, redir_lst));
}

t_cmd	*ast_to_cmd(t_ast *root)
{
	if (!root)
		return (NULL);
	if (root->type == AST_PIPE)
		return (flatten_pipe(root));
	else if (root->type == AST_CMD || root->type == AST_SUBSHELL
		|| root->type == AST_REDIR)
		return (switch_type(root));
	free_ast(root);
	return (NULL);
}
