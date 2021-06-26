NAME			=	pipex

SRCS_DIR		=	./srcs/

OBJS_DIR		=	./objs/

HEADERS_DIR		=	./includes/

SRCS			=	$(SRCS_DIR)error.c \
					$(SRCS_DIR)execute_cmd.c \
					$(SRCS_DIR)here_doc.c \
					$(SRCS_DIR)pipes.c \
					$(SRCS_DIR)pipex.c

HEADERS			=	$(wildcard $(HEADERS_DIR)*.h)

OBJS			=	$(addprefix $(OBJS_DIR), $(notdir $(SRCS:.c=.o)))


CC				=	gcc

CFLAGS			=	-Wall -Wextra -Werror -O3

RM				=	rm -f

LIBFT_DIR		=	./libft/

LIBFT			=	$(addprefix $(LIBFT_DIR), libft.a)

all: $(LIBFT) $(NAME)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c $(HEADERS) $(LIBFT) | $(OBJS_DIR)
	$(CC) $(CFLAGS) -I $(HEADERS_DIR) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ -I $(HEADERS_DIR) $^


clean:
		$(RM) -r $(OBJS_DIR)
		make -C $(LIBFT_DIR) clean

fclean: clean
		$(RM) $(NAME)
		make -C $(LIBFT_DIR) fclean

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

re: fclean $(OBJS_DIR) all

.PHONY: all clean fclean re
