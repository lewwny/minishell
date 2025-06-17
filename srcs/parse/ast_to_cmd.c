/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_to_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:58:57 by macauchy          #+#    #+#             */
/*   Updated: 2025/06/17 14:32:21 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*flatten_pipe(t_ast *root)
{
	t_cmd	*l_lst;
	t_cmd	*r_lst;
	t_cmd	*it;

	it = NULL;
	l_lst = ast_to_cmd(root->ast.pipe.left);
	r_lst = ast_to_cmd(root->ast.pipe.right);
	if (!l_lst)
		return (r_lst);
	it = l_lst;
	while (it->next)
		it = it->next;
	it->next = r_lst;
	return (l_lst);
}

t_ast	*gather_redirs(t_ast *node, t_redir **rlist)
{
	t_redir	*r;
	t_ast	*child;

	r = NULL;
	child = NULL;
	if (!node)
		return (NULL);
	if (node->type == AST_REDIR)
	{
		r = (t_redir *)malloc(sizeof(t_redir));
		if (!r)
			return (NULL);
		r->type = node->ast.redir.type;
		r->target = ft_strdup(node->ast.redir.target);
		r->next = *rlist;
		*rlist = r;
		child = node->ast.redir.child;
		return (gather_redirs(child, rlist));
	}
	return (node);
}

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

t_cmd	*switch_type(t_ast *root)
{
	t_redir	*redir_lst;
	t_ast	*leaf;
	t_cmd	*cmd;

	redir_lst = NULL;
	leaf = gather_redirs(root, &redir_lst);
	cmd = NULL;
	if (!leaf)
	{
		free_redir(redir_lst);
		return (NULL);
	}
	if (leaf->type == AST_PIPE)
	{
		return (flatten_pipe_on_redir(leaf, redir_lst));
	}
	cmd = new_cmd();
	cmd->redirs = redir_lst;
	if (leaf->type == AST_CMD)
		cmd->args = copy_args(leaf->ast.cmd.args);
	else
	{
		cmd->args = (char **)malloc(sizeof(char *));
		if (!cmd->args)
			return (NULL);
	}
	return (cmd);
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
