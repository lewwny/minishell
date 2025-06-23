# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/07 14:49:00 by lenygarcia        #+#    #+#              #
#    Updated: 2025/06/23 15:24:14 by macauchy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g

RESET 		= \033[0m
GREEN 		= \033[0;32m
BLUE   		= \033[0;34m
YELLOW		= \033[1;33m

FILES		= builtins/builtins.c builtins/builtins_func.c builtins/cd_builtins.c \
			builtins/export_builtins.c builtins/export_utils.c builtins/ls_builtins.c \
			builtins/export_utils2.c \
			\
			exec/exec.c exec/exec_utils.c exec/path.c exec/redir.c exec/get_cmd.c \
			exec/heredoc.c exec/exec_cmds.c\
			\
			env/env.c env/env_utils.c env/expand.c env/oldpwd.c env/create_env_node.c \
			\
			utils/free_parsing.c utils/free_utils.c utils/free.c utils/lst_utils.c \
			utils/prompt.c utils/signal.c utils/singleton.c utils/utils.c utils/error.c \
			\
			parse/ast_to_cmd.c parse/extract_data.c parse/parse_expr.c \
			parse/parse_infix.c parse/parse_prefix.c parse/parse_utils.c \
			parse/parsing.c parse/pratt_handler.c parse/pratt_parsing.c \
			parse/tokenizer.c parse/tokenizer2.c parse/ast_utils.c \
			parse/handler_utils.c parse/args_ctx.c parse/collect_word.c\
			parse/ast_to_cmd_utils.c parse/collect_word_utils.c \
			parse/pratt_parsing_utils.c parse/pratt_parsing_utils2.c \
			parse/parse_utils2.c parse/pratt_parsing_utils3.c\
			\
			main.c
SRC_DIR		= srcs
SRCS		= $(addprefix $(SRC_DIR)/, $(FILES))
OBJDIR		= .obj
OBJS  		= $(SRCS:srcs/%.c=$(OBJDIR)/%.o)
NAME  		= minishell
HEADER		= includes/minishell.h
LIBFT_PATH	= ./libft
LIBFT 		= $(LIBFT_PATH)/libft.a

all: $(OBJDIR) $(NAME)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(NAME): $(OBJS) $(LIBFT)
	@printf "$(GREEN)==> Linking $(NAME)$(RESET)\n"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline
	@printf "$(GREEN)✔ Build complete!$(RESET)\n"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH) --no-print-directory >/dev/null

$(OBJDIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	@mkdir -p $(dir $@)
	@printf "$(YELLOW)==> Compiling $(notdir $<)$(RESET)\n"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@printf "$(BLUE)==> Cleaning objects$(RESET)\n"
	@rm -rf $(OBJDIR)
	@$(MAKE) -C $(LIBFT_PATH) --no-print-directory clean >/dev/null

fclean: clean
	@printf "$(BLUE)==> Cleaning all$(RESET)\n"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_PATH) --no-print-directory fclean >/dev/null

re: fclean all

.PHONY: all clean fclean re
