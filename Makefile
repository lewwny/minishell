# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/07 14:49:00 by lenygarcia        #+#    #+#              #
#    Updated: 2025/06/12 17:06:50 by lengarci         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

CC    		= cc
CFLAGS		= -Wall -Wextra -g3 -march=native

RESET 		= \033[0m
GREEN 		= \033[0;32m
BLUE   		= \033[0;34m
YELLOW		= \033[1;33m

FILES		= main.c utils.c free.c path.c error.c						\
			singleton.c parsing.c exec.c								\
			builtins.c env.c builtins_func.c							\
			export_builtins.c lst_utils.c free_utils.c					\
			prompt.c signal.c ls_builtins.c expand.c pratt_parsing.c	\
			pratt_handler.c extract_data.c tokenizer.c					\
			tokenizer2.c parse_expr.c parse_prefix.c parse_infix.c		\
			parse_utils.c ast_to_cmd.c exec_utils.c env_utils.c \
			create_env_node.c export_utils.c
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
