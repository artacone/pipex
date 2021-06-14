# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rvertie <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/12 21:13:42 by rvertie           #+#    #+#              #
#    Updated: 2021/06/02 18:37:48 by rvertie          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libft.a

SRC_DIR		= ./srcs/

OBJ_DIR		= ./objs/

HEADER_DIR	= ./includes/

SRCS		= $(wildcard ${SRC_DIR}*.c)

HEADERS		= $(wildcard ${HEADER_DIR}*.h)

OBJS		= $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.c=.o)))

CC			= gcc

CFLAGS		= -Wall -Wextra -Werror -O3

RM			= rm -f


.PHONY: all clean fclean re

all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I $(HEADER_DIR) -c $< -o $@

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

re: fclean  $(OBJ_DIR) all
