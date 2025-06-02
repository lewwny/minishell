# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lenygarcia <marvin@42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/07 14:49:00 by lenygarcia        #+#    #+#              #
#    Updated: 2025/06/02 08:43:12 by lengarci         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

CC    		= cc
CFLAGS		= -Wall -Wextra -Werror -g

RESET 		= \033[0m
GREEN 		= \033[0;32m
BLUE   		= \033[0;34m
YELLOW		= \033[1;33m

SRCS   	 	= srcs/main.c srcs/utils.c
OBJS  		= $(SRCS:.c=.o)
NAME  		= minishell
INCLUDES	= -Iincludes -Ilibft/includes
LIBFT_PATH	= ./libft
LIBFT 		= $(LIBFT_PATH)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@printf "$(GREEN)==> Linking $(NAME)$(RESET)\n"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline
	@printf "$(GREEN)✔ Build complete!$(RESET)\n"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

%.o: %.c
	@printf "$(YELLOW)==> Compiling $<$(RESET)\n"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@printf "$(BLUE)==> Cleaning objects$(RESET)\n"
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	@printf "$(BLUE)==> Cleaning all$(RESET)\n"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re
