/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_to_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:52:25 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/23 11:53:44 by macauchy         ###   ########.fr       */
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
			malloc_error();
		r->type = node->ast.redir.type;
		r->target = ft_strdup(node->ast.redir.target);
		if (!r->target)
			malloc_error();
		redir_add_back(rlist, r);
		child = node->ast.redir.child;
		return (gather_redirs(child, rlist));
	}
	return (node);
}
