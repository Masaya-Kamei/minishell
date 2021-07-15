/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 09:39:27 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 16:46:00 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <errno.h>
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>

# define ENV 0
# define SHELL 1
# define SPECIAL 2
# define SUCCESS 0
# define ERR_MALLOC 1
# define ERR_INVALID_OP 2
# define ERR_NOSET_HOME 3
# define ERR_NOSET_OLDPWD 4
# define ERR_NUM_ARG_REQ 5
# define ERR_TOO_MANY_ARG 6
# define ERR_INVALID_ID 7
# define ERR_INVALID_OP_ARG 8

typedef enum e_str_type{
	RAW			= 'R',
	S_QUOTE		= '\'',
	D_QUOTE		= '\"'
}			t_str_type;

typedef enum e_err_num_type{
	ERRNO		= 0,
	ORIGINAL	= 1
}			t_err_num_type;

typedef enum e_token_type{
	PIPE		= '|',
	GREATER		= '>',
	LESS		= '<',
	D_GREATER	= 'G',
	D_LESS		= 'L',
	WORD		= 'W'
}			t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
}				t_token;

int		lex_line(char *line, t_token **tokens, int *token_num);
int		expand_word_token(char **word, t_list *vars_list[3]);
int		mini_ehco(char **argv);
int		mini_cd(char **argv, t_list *vars_list[3]);
int		mini_pwd(char **argv);
int		mini_exit(char **argv);
int		mini_env(char **argv, t_list *vars_list[3]);
int		mini_export(char **argv, t_list *vars_list[3]);
int		mini_unset(char **argv, t_list *vars_list[3]);
int		check_valid_identifier(char *var, int var_name_len);
t_list	*create_env_list(char **envp);
char	**create_envp(t_list *env_list);
t_list	*lstnew_with_strdup(char *str);
t_list	*get_target_list(t_list *any_list, char *var, int var_name_len);
int		add_new_var(t_list **any_list, char *var);
char	*get_var(t_list *vars_list[3], char *var_name);
int		set_var(t_list *vars_list[3], char *var, int var_type);
void	delete_var(t_list *vars_list[3], char *var_name, int var_type);
char	*get_current_absolute_path(void);
int		set_pwd_var(t_list *vars_list[3], int init);
int		set_oldpwd_var(t_list *vars_list[3], int init);
void	set_exit_status(t_list *special_list, int exit_status);
int		countup_shlvl_env(t_list **env_list);
void	write_err(char *word, int err_num, t_err_num_type type);
int		write_shell_err(char *word, int err_num, t_err_num_type type);
void	free_double_pointer(char **strs);
void	free_tokens(t_token *tokens);
int		free_and_return(void *p, int status);
void	free_and_fill_null(char **p);
void	clear_vars_list(t_list *vars_list[3]);
void	print_line_and_word_start_array(char *line, int *word_start);
void	print_tokens(t_token *tokens, t_list *vars_list[3]);
void	execve_sleep(void);
void	test_vars_list(t_list *vars_list[3]);

int		g_received_signal;

#endif
