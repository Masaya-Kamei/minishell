/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 09:39:27 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/25 12:53:39 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <dirent.h>
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_bool
{
	FALSE		= 0,
	TRUE		= 1
}			t_bool;

typedef enum e_str_type{
	RAW			= 'R',
	S_QUOTE		= '\'',
	D_QUOTE		= '\"'
}			t_str_type;

typedef enum e_status
{
	SUCCESS 			= 0,
	E_INVALID_OP 		= 1,
	E_NOSET_VAR 		= 2,
	E_NUM_ARG_REQ 		= 3,
	E_TOO_MANY_ARG 		= 4,
	E_INVALID_ID 		= 5,
	E_INVALID_OP_ARG	= 6,
	E_SYNTAX 			= 7,
	E_SIGNAL			= 8,
	E_MALLOC			= 9,
	E_DUP_CLOSE			= 10,
	E_CHDIR				= 11,
	E_FORK				= 12,
	E_OPEN				= 13,
	E_UNLINK			= 14,
	E_EXECVE			= 15,
	E_EXTERNAL			= 16
}			t_status;

typedef enum e_err_place
{
	P_SHELL		= 0,
	P_ECHO		= 1,
	P_CD		= 2,
	P_PWD		= 3,
	P_EXPORT	= 4,
	P_UNSET		= 5,
	P_ENV		= 6,
	P_EXIT		= 7,
	P_EXTERNAL	= 8
}			t_err_place;

typedef enum e_vars_type{
	ENV			= 0,
	SHELL		= 1,
	SPECIAL		= 2
}			t_vars_type;

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

typedef int	t_exit_status;

int			g_received_signal;

t_status		lex_line(char *line, t_token **tokens, int *token_num);
t_status		expand_word_token(char **word, t_list *vars_list[3]);
t_status		start_process(
					t_token *tokens, int start, int end, t_list *vars_list[3]);
t_exit_status	mini_echo(char **argv, t_list *vars_list[3]);
t_exit_status	mini_cd(char **argv, t_list *vars_list[3]);
t_exit_status	mini_pwd(char **argv, t_list *vars_list[3]);
t_exit_status	mini_exit(char **argv, t_list *vars_list[3]);
t_exit_status	mini_env(char **argv, t_list *vars_list[3]);
t_exit_status	mini_export(char **argv, t_list *vars_list[3]);
t_exit_status	mini_unset(char **argv, t_list *vars_list[3]);
t_bool			check_valid_identifier(char *var, int var_name_len);
t_list			*create_env_list(char **envp);
char			**create_envp(t_list *env_list);
t_list			*lstnew_with_strdup(char *str);
t_list			*get_target_list(t_list *any_list, char *var, int var_name_len);
t_status		add_new_var(t_list **any_list, char *var);
char			*get_var(t_list *vars_list[3], char *var_name);
t_status		set_var(t_list *vars_list[3], char *var, int var_type);
void			delete_var(t_list *vars_list[3], char *var_name, int var_type);
char			*get_current_absolute_path(void);
t_status		set_pwd_var(t_list *vars_list[3], int init);
t_status		set_oldpwd_var(t_list *vars_list[3], int init);
void			set_exit_status(t_list *special_list, int exit_status);
t_status		countup_shlvl_env(t_list **env_list);
t_exit_status	get_exit_status_with_errout(
					char *word, t_status status, t_err_place err_place);
void			free_double_pointer(void **strs);
void			free_tokens(t_token *tokens);
t_status		free_and_return(void *p, t_status status);
void			free_and_fill_null(char **p);
void			clear_vars_list(t_list *vars_list[3]);
void			print_line_and_word_start_array(char *line, int *word_start);
void			print_tokens(t_token *tokens, t_list *vars_list[3]);
void			execve_sleep(void);
void			test_vars_list(t_list *vars_list[3]);
t_status		debug_process_command(
					t_token *tokens, int start, int end, t_list *vars_list[3]);
t_status		process_command(t_token *tokens, int start_index, int end_index, t_list *vars_list[3]);
t_status		get_command_argv(t_token *tokens, char ***command, int start_index, int end_index);
t_status		redirect_list(t_token *tokens, int start_index, int end_index, int **save_fd);
char			*get_command_path(char *cmd);

#endif
