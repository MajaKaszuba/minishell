# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/27 15:43:33 by mkaszuba          #+#    #+#              #
#    Updated: 2024/12/03 15:06:02 by mkaszuba         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = @cc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(SRC_DIR)/main.c \
		$(SRC_DIR)/utils.c \
		$(SRC_DIR)/parser.c \
		$(SRC_DIR)/executor.c \
		$(SRC_DIR)/builtin.c \
		$(SRC_DIR)/signals.c \
		$(SRC_DIR)/redir.c	\

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo ""
	@echo "✨ minishell created ✨"
	@echo ""
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline

$(LIBFT):
	@make -s -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@make clean -s -C $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR)
	@echo ""
	@echo "🗑️  deleted 🗑️"
	@echo ""

fclean: clean
	@rm -f $(NAME)
	@make fclean -s -C $(LIBFT_DIR)

re:	fclean all

.PHONY: all clean fclean re