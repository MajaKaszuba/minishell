# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/27 15:43:33 by mkaszuba          #+#    #+#              #
#    Updated: 2024/11/27 16:10:13 by mkaszuba         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
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

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Reguła główna: kompiluje minishell
all: $(NAME)

# Kompilacja minishell
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline

# Budowanie libft
$(LIBFT):
	@make -s -C $(LIBFT_DIR)

# Kompilacja obiektów w katalogu obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Czyszczenie plików obiektowych
clean:
	rm -f $(OBJS)
	@make clean -s -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)

# Czyszczenie wszystkiego
fclean: clean
	rm -f $(NAME)
	@make fclean -s -C $(LIBFT_DIR)

# Przebudowa
re: fclean all
