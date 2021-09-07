SRCNAME	:=	minishell.c lex_line.c expand_word1.c expand_word2.c \
			start_process.c process_pipeline.c \
			process_command1.c process_command2.c process_redirect.c \
			var/var_ope.c var/var_set_any.c var/var_utils.c \
			builtins/mini_echo.c builtins/mini_cd.c builtins/mini_pwd.c \
			builtins/mini_env.c builtins/mini_export.c builtins/mini_unset.c \
			builtins/mini_exit.c \
			utils/debug.c utils/free.c utils/error.c utils/utils.c \
			utils/list_utils.c
SRC_DIR := ./srcs/
SRCS	:= $(addprefix $(SRC_DIR), $(SRCNAME))
OBJS	:= $(SRCS:.c=.o)
INCLUDE := -I./include/
NAME	:= minishell

LIBFT		:= ./libft/libft.a
LIBFTTARGET	:= all

CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror
RM		:= rm -rf

LIBFTDIR	:= ./libft
LIBFTNAME 	:= ft
LIBFTINCLUDEDIR := ./libft

LIBREADDIR			:= $(shell brew --prefix readline)/lib
LIBREADNAME			:= readline
LIBHISTNAME			:= history
LIBREADINCLUDEDIR	:= $(shell brew --prefix readline)/include

LIBINCLUDE	:= -I${LIBFTINCLUDEDIR} -I${LIBREADINCLUDEDIR}
LIBDIR		:= -L${LIBFTDIR} -L${LIBREADDIR}
LIBLINK		:= -l${LIBFTNAME} -l${LIBREADNAME} -l${LIBHISTNAME}

all		:	$(NAME)

.c.o	:
			$(CC) $(CFLAGS) $(INCLUDE) $(LIBINCLUDE) -c $< -o $(<:.c=.o)

$(NAME)	:	$(LIBFT) $(OBJS)
			$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBINCLUDE) $(LIBDIR) $(LIBLINK) -o $(NAME)

$(LIBFT):
			make $(LIBFTTARGET) -C $(LIBFTDIR)

clean	:
			$(RM) Makefile.bak $(OBJS)
			make clean -C $(LIBFTDIR)

fclean	: 	clean
			$(RM) $(NAME)
			make fclean -C $(LIBFTDIR)

re		:	fclean all

debug	: CFLAGS += -g
debug	: LIBFTTARGET := debug
debug	: re

address	: CFLAGS += -g -fsanitize=address
address	: LIBFTTARGET := address
address	: re

.PHONY:	all clean fclean re debug address
