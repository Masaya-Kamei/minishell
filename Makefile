vpath   %.c srcs:srcs/var:srcs/utils:srcs/builtins

SRCSNAME:=	minishell.c lex_line.c expand_word1.c expand_word2.c \
			start_process.c process_pipeline.c \
			process_command1.c process_command2.c process_redirect.c \
			var_ope.c var_set_any.c var_utils.c \
			mini_echo.c mini_cd.c mini_pwd.c mini_env.c mini_export.c \
			mini_unset.c mini_exit.c \
			debug.c free.c error.c utils.c list_utils.c
SRCSDIR := ./srcs/
SRCS	:= $(addprefix $(SRCSDIR), $(SRCSNAME))

OBJSDIR	:=	./objs
OBJS	:=	$(addprefix $(OBJSDIR)/, $(SRCSNAME:.c=.o))
DEPS    :=  $(addprefix $(OBJSDIR)/, $(SRCSNAME:.c=.d))

INCLUDE := -I./include/
NAME	:= minishell

LIBFT		:= ./libft/libft.a
LIBFTTARGET	:= all

CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror
CFLAGS	+= -MMD -MP
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

$(NAME)	:	$(LIBFT) $(OBJS)
			$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBINCLUDE) $(LIBDIR) $(LIBLINK) -o $(NAME)

$(OBJSDIR)/%.o	:	%.c
			@mkdir -p $(dir $@)
			$(CC) $(CFLAGS) $(INCLUDE) $(LIBINCLUDE) -o $@ -c $<

-include $(DEPS)

$(LIBFT):
			make $(LIBFTTARGET) -C $(LIBFTDIR)

clean	:
			$(RM) $(OBJS) $(DEPS)
			make clean -C $(LIBFTDIR)

fclean	: 	clean
			$(RM) $(NAME)
			make fclean -C $(LIBFTDIR)

re		:	fclean all

debug	: CFLAGS		+= -g
debug	: LIBFTTARGET	:= debug
debug	: re

address	: CFLAGS		+= -g -fsanitize=address
address	: LIBFTTARGET	:= address
address	: re

ifeq ($(shell uname),Darwin)
leak	:	CC			=	/usr/local/opt/llvm/bin/clang
endif
leak	: CFLAGS		+=	-g -fsanitize=leak
leak	: LIBFTTARGET	:= leak
leak	: re

.PHONY:	all clean fclean re debug address leak
