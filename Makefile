# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/07 14:49:00 by lenygarcia        #+#    #+#              #
#    Updated: 2025/06/10 12:24:27 by macauchy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

CC    		= cc
CFLAGS		= -Wall -Wextra -Werror -g

RESET 		= \033[0m
GREEN 		= \033[0;32m
BLUE   		= \033[0;34m
YELLOW		= \033[1;33m

FILES		= main.c utils.c free.c path.c error.c \
			singleton.c parsing.c exec.c \
			builtins.c env.c builtins_func.c \
			export_builtins.c lst_utils.c free_utils.c \
			prompt.c signal.c ls_builtins.c expand.c pratt_parsing.c \
			pratt_handler.c extract_data.c
SRC_DIR		= srcs
SRCS		= $(addprefix $(SRC_DIR)/, $(FILES))
OBJDIR		= .obj
OBJS  		= $(SRCS:srcs/%.c=$(OBJDIR)/%.o)
NAME  		= minishell
HEADER		= includes/minishell.h
# INCLUDES	= -Iincludes -Ilibft/includes
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
