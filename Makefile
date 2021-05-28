# Makefile for minishell, updated 2021年 5月27日 木曜日 17時43分54秒 JST

SRCNAME	:= main.c

# DO NOT ADD OR MODIFY ANY LINES ABOVE THIS -- run 'make source' to add files

SRC_DIR := ./srcs/
SRCS	:= $(addprefix $(SRC_DIR), $(SRCNAME))
OBJS	:= $(SRCS:.c=.o)
INCLUDE := -I./include/
NAME	:= minishell

LIBFTDIR	:= ./libft
LIBFTNAME 	:= libft.a
LIBFT		:= $(LIBFTDIR)/$(LIBFTNAME)

CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror
RM		:= rm -rf

all		:	$(NAME)

.c.o	:
			$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(<:.c=.o)

$(NAME)	:	$(LIBFTNAME) $(OBJS)
			$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBFT) -o $(NAME)


$(LIBFTNAME):
			make -C $(LIBFTDIR)

clean	:
			$(RM) Makefile.bak $(OBJS)
			make clean -C $(LIBFTDIR)

fclean	: 	clean
			$(RM) $(NAME)
			make fclean -C $(LIBFTDIR)

re		:	fclean all

source:
	@cp Makefile Makefile.bak
	@sed -i "" -r -e "s/(^# Makefile .* updated).*/\1 `date`/" Makefile
	@sed -i "" -r -e "s/(^SRCNAME\t:=).*/\1 `ls srcs`/" Makefile

.PHONY:	all clean fclean re source $(LIBFTNAME)
