/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:01:28 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/20 15:36:01 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_ctx_str(void)
{
	t_data	*m;
	size_t	i;

	m = _data();
	i = 0;
	if (!m->ctx)
		return ;
	while (m->ctx[i].arg)
	{
		free(m->ctx[i].arg);
		i++;
	}
	free(m->ctx);
	m->ctx = NULL;
}

static void	ensure_capacityi(unsigned int *cap, unsigned int *c, t_data *m)
{
	size_t	i;

	i = *cap;
	if (*c + 1 >= *cap)
	{
		if (*cap == 0)
			*cap = 8;
		else
			*cap *= 2;
		m->args = ft_realloc(m->args, (sizeof(char *) * (*c)), \
			sizeof(char *) * (*cap));
		m->ctx = ft_realloc(m->ctx, (sizeof(t_ctx) * (*c)), \
			sizeof(t_ctx) * (*cap));
		if (!m->args || !m->ctx)
		{
			ft_dprintf(2, "Error: Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
		while (i < *cap)
		{
			m->ctx[i].arg = NULL;
			m->ctx[i].is_escaped = false;
			i++;
		}
	}
}

void	append_t_ctx(unsigned int *cap, unsigned int *c, t_ctx *new_tok)
{
	t_data	*m;

	m = _data();
	ensure_capacityi(cap, c, m);
	m->args[*c] = new_tok->arg;
	if (m->ctx[*c].arg)
		free(m->ctx[*c].arg);
	m->ctx[*c].arg = ft_strdup(new_tok->arg);
	if (!m->ctx[*c].arg)
		malloc_error();
	m->ctx[*c].is_escaped = new_tok->is_escaped;
	(*c)++;
}
