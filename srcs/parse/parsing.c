/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:38:58 by lengarci          #+#    #+#             */
/*   Updated: 2025/06/19 09:39:07 by lengarci         ###   ########.fr       */
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

#define INDENT_WIDTH 4

static void     print_indent(int level)
{
        for (int i = 0; i < level * INDENT_WIDTH; i++)
                putchar(' ');
}

void    print_redirection_list(const t_redir *redir)
{
        const t_redir *r = redir;
        while (r)
        {
                printf("  - redir: ");
                switch (r->type)
                {
                        case TK_DUP_IN:    printf("dup_in "); break;
                        case TK_DUP_OUT:   printf("dup_out "); break;
                        case TK_REDIR_IN:  printf("redir_in "); break;
                        case TK_REDIR_OUT: printf("redir_out "); break;
                        case TK_APPEND:    printf("append "); break;
                        case TK_HEREDOC:   printf("heredoc "); break;
                        case TK_HERESTRING:printf("herestring "); break;
                        case TK_BACKGROUND:printf("background "); break;
                        default:           printf("unknown ");
                }
                printf("target: '%s'\n", r->target);
                r = r->next;
        }
}

void    print_ast(const t_ast *node, int indent)
{
        if (!node)
        {
                print_indent(indent);
                printf("(null)\n");
                return;
        }

        print_indent(indent);
        switch (node->type)
        {
                case AST_CMD:
                        printf("AST_CMD: argv = ");
                        for (int i = 0; node->ast.cmd.args && node->ast.cmd.args[i]; i++)
                                printf("'%s' ", node->ast.cmd.args[i]);
                        printf("\n");
                        break;

                case AST_PIPE:
                        printf("AST_PIPE\n");
                        print_indent(indent); printf("├── left:\n");
                        print_ast(node->ast.pipe.left, indent + 1);
                        print_indent(indent); printf("└── right:\n");
                        print_ast(node->ast.pipe.right, indent + 1);
                        break;

                case AST_REDIR:
                        printf("AST_REDIR: op = ");
                        switch (node->ast.redir.type)
                        {
                                case TK_REDIR_IN:  printf("<"); break;
                                case TK_REDIR_OUT: printf(">"); break;
                                case TK_APPEND:    printf(">>"); break;
                                case TK_HEREDOC:   printf("<<"); break;
                                case TK_HERESTRING:printf("<<<"); break;
                                case TK_DUP_IN:    printf(">&"); break;
                                case TK_DUP_OUT:   printf("<&"); break;
                                case TK_BACKGROUND:printf("&"); break;
                                case TK_PIPE:      printf("|"); break;
                                case TK_AND:       printf("&&"); break;
                                case TK_OR:        printf("||"); break;
                                case TK_SEMICOLON: printf(";"); break;
                                default:           printf("?"); break;
                        }
                        printf(", target = '%s'\n", node->ast.redir.target);
                        print_indent(indent); printf("└── child:\n");
                        print_ast(node->ast.redir.child, indent + 1);
                        break;

                case AST_SUBSHELL:
                        printf("AST_SUBSHELL\n");
                        print_ast(node->ast.subshell.sub, indent + 1);
                        break;


                case AST_SEQ:
                        printf("AST_SEQ\n");
                        print_indent(indent); printf("├── left:\n");
                        print_ast(node->ast.logical.left, indent + 1);
                        print_indent(indent); printf("└── right:\n");
                        print_ast(node->ast.logical.right, indent + 1);
                        break;

                case AST_BG:
                        printf("AST_BG\n");
                        print_indent(indent); printf("└── child:\n");
                        print_ast(node->ast.logical.left, indent + 1);
                        break;

                case AST_AND:
                        printf("AST_AND\n");
                        print_indent(indent); printf("├── left:\n");
                        print_ast(node->ast.logical.left, indent + 1);
                        print_indent(indent); printf("└── right:\n");
                        print_ast(node->ast.logical.right, indent + 1);
                        break;

                case AST_OR:
                        printf("AST_OR\n");
                        print_indent(indent); printf("├── left:\n");
                        print_ast(node->ast.logical.left, indent + 1);
                        print_indent(indent); printf("└── right:\n");
                        print_ast(node->ast.logical.right, indent + 1);
                        break;

                default:
                        print_indent(indent);
                        printf("UNKNOWN AST NODE\n");
                        break;
        }
}

static void	reverse_redir_list(t_redir **redirs)
{
	t_redir	*prev = NULL;
	t_redir	*current = *redirs;
	t_redir	*next;

	while (current != NULL)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	*redirs = prev;
}

void	parsing(char *input)
{
	_data()->escaped = 0;
	_data()->args = split_on_whitespace(input);
	if (!_data()->args || !_data()->ctx || _data()->early_error)
	{
		_data()->exit_code = 1;
		free_ctx();
		return ;
	}
	expand_env_vars_in_ctx();
	_data()->tokens = tokenize_to_pratt(_data()->ctx);
	_data()->ast = parse_expression(0);
	if (_data()->early_error || _data()->error)
	{
		_data()->exit_code = 1;
		free_ast(_data()->ast);
		_data()->ast = NULL;
	}
	// print_ast(_data()->ast, 0);
	if (_data()->ast)
		_data()->cmds = ast_to_cmd(_data()->ast);
	if (!_data()->early_error || !_data()->error)
		reverse_redir_list(&_data()->cmds->redirs);
	free_ctx();
	free_ast(_data()->ast);
	free_split(_data()->args);
	free_token_array();
}
