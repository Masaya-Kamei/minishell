# Makefile for minishell, updated 2021年 6月 3日 木曜日 11時44分21秒 JST

SRCNAME	:= debug.c get_line.c lex_line.c minishell.c utils.c

# DO NOT ADD OR MODIFY ANY LINES ABOVE THIS -- run 'make source' to add files

SRC_DIR := ./srcs/
SRCS	:= $(addprefix $(SRC_DIR), $(SRCNAME))
OBJS	:= $(SRCS:.c=.o)
INCLUDE := -I./include/
NAME	:= minishell

LIBFTDIR	:= ./libft
LIBFTNAME 	:= libft.a
LIBFT		:= $(LIBFTDIR)/$(LIBFTNAME)
LIBFTTARGET	:= bonus

CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror
RM		:= rm -rf

all		:	$(NAME)

.c.o	:
			$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(<:.c=.o)

$(NAME)	:	$(LIBFTNAME) $(OBJS)
			$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFTNAME):
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

.PHONY:	all clean fclean re debug address source $(LIBFTNAME)
