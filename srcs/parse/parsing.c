/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:38:58 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/15 15:03:39 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_ctx(void)
{
	int	i;

	i = 0;
	if (!_data()->ctx || !_data()->ctx[i].arg)
		return ;
	while (_data()->ctx[i].arg)
	{
		free(_data()->ctx[i].arg);
		_data()->ctx[i].arg = NULL;
		i++;
	}
	free(_data()->ctx);
	_data()->ctx = NULL;
}

// void	print_redirection_list(const t_redir *redir)
// {
// 	const t_redir *r = redir;
// 	while (r)
// 	{
// 		printf("  - redir: ");
// 		switch (r->type)
// 		{
// 			case TK_DUP_IN:    printf("dup_in "); break;
// 			case TK_DUP_OUT:   printf("dup_out "); break;
// 			case TK_REDIR_IN:  printf("redir_in "); break;
// 			case TK_REDIR_OUT: printf("redir_out "); break;
// 			case TK_APPEND:    printf("append "); break;
// 			case TK_HEREDOC:   printf("heredoc "); break;
// 			case TK_HERESTRING:printf("herestring "); break;
// 			case TK_BACKGROUND:printf("background "); break;
// 			default:           printf("unknown ");
// 		}
// 		printf("target: '%s'\n", r->target);
// 		r = r->next;
// 	}
// }

// void	print_cmd_list(const t_cmd *cmdlist)
// {
// 	int i = 0;
// 	while (cmdlist)
// 	{
// 		printf("== CMD #%d ==\n", i++);
// 		if (cmdlist->args)
// 		{
// 			printf("args: ");
// 			for (int j = 0; cmdlist->args[j]; j++)
// 				printf("'%s' ", cmdlist->args[j]);
// 			printf("\n");
// 		}
// 		else
// 			printf("args: (null)\n");

// 		if (cmdlist->redirs)
// 		{
// 			printf("redirections:\n");
// 			print_redirection_list(cmdlist->redirs);
// 		}
// 		else
// 			printf("redirections: (none)\n");

// 		cmdlist = cmdlist->next;
// 		printf("\n");
// 	}
// }

// void	print_token_array(t_token *tokens)
// {
// 	printf("== TOKENS ==\n");
// 	for (int i = 0; tokens[i].type != TK_EOF && tokens[i].type != TK_ERROR; i++)
// 	{
// 		printf("[%2d] type: ", i);
// 		switch (tokens[i].type)
// 		{
// 			case TK_WORD:        printf("WORD        "); break;
// 			case TK_PIPE:        printf("PIPE        "); break;
// 			case TK_REDIR_IN:    printf("REDIR_IN    "); break;
// 			case TK_REDIR_OUT:   printf("REDIR_OUT   "); break;
// 			case TK_APPEND:      printf("APPEND      "); break;
// 			case TK_HEREDOC:     printf("HEREDOC     "); break;
// 			case TK_LPAREN:      printf("LPAREN      "); break;
// 			case TK_RPAREN:      printf("RPAREN      "); break;
// 			case TK_EOF:         printf("EOF         "); break;
// 			case TK_ERROR:       printf("ERROR       "); break;
// 			case TK_DUP_IN:      printf("DUP_IN      "); break;
// 			case TK_DUP_OUT:     printf("DUP_OUT     "); break;
// 			case TK_BACKGROUND:  printf("BACKGROUND  "); break;
// 			case TK_HERESTRING:  printf("HERESTRING  "); break;
// 			case TK_SEMICOLON:   printf("SEMICOLON   "); break;
// 			case TK_AND:         printf("AND         "); break;
// 			case TK_OR:          printf("OR          "); break;
// 			default:             printf("UNKNOWN     "); break;
// 		}
// 		printf("text: %-16s lbp: %-2d rbp: 
//			%-2d\n", tokens[i].text, tokens[i].left_bp, tokens[i].right_bp);
// 	}
// 	printf("[EOF]\n\n");
// }

static void	expand_env_vars_in_ctx(void)
{
	int		i;
	char	*temp;

	i = 0;
	if (!_data()->ctx)
		return ;
	while (_data()->ctx[i].arg)
	{
		if (ft_strchr(_data()->ctx[i].arg, '$')
			&& _data()->ctx[i].is_escaped == false)
		{
			temp = _data()->ctx[i].arg;
			_data()->ctx[i].arg = replace_env_vars(temp);
			free(temp);
		}
		i++;
	}
}

void	parsing(char *input)
{
	_data()->escaped = 0;
	_data()->args = split_on_whitespace(input);
	if (!_data()->args || !_data()->ctx)
		return ;
	expand_env_vars_in_ctx();
	_data()->tokens = tokenize_to_pratt(_data()->ctx);
	_data()->ast = parse_expression(0);
	_data()->cmds = ast_to_cmd(_data()->ast);
	free_ctx();
	free_ast(_data()->ast);
	free_split(_data()->args);
	free_token_array();
}
