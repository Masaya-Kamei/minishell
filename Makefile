SRCNAME	:=	debug.c expand_word.c lex_line.c minishell.c free.c error.c utils.c \
			process_pipeline.c process_command.c process_command_utils.c process_redirect.c \
			var/var_env.c var/var_ope.c var/var_set_any.c var/var_utils.c \
			builtins/mini_echo.c builtins/mini_cd.c builtins/mini_pwd.c builtins/mini_env.c \
			builtins/mini_export.c builtins/mini_unset.c builtins/mini_exit.c
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

LIBREADDIR			:= /usr/local/opt/readline/lib
LIBREADNAME			:= readline
LIBHISTNAME			:= history
LIBREADINCLUDEDIR	:= /usr/local/opt/readline/include

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
