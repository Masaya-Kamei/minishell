# Makefile for minishell, updated 2021年 6月11日 金曜日 16時49分04秒 JST

SRCNAME	:= debug.c lex_line.c minishell.c utils.c change_word_to_string.c

# DO NOT ADD OR MODIFY ANY LINES ABOVE THIS -- run 'make source' to add files

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

source:
	@cp Makefile Makefile.bak
	@sed -i "" -r -e "s/(^# Makefile .* updated).*/\1 `date`/" Makefile
	@sed -i "" -r -e "s/(^SRCNAME\t:=).*/\1 `ls -1 srcs | grep .c | xargs`/" Makefile

.PHONY:	all clean fclean re debug address source
