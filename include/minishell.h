/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 09:39:27 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/15 12:13:07 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>

# define GETCWD_EMSG "error retrieving current directory:\
 getcwd: cannot access parent directories"
# define OVER_INT_EMSG "file descriptor out of range"

typedef enum e_bool
{
	FALSE		= 0,
	TRUE		= 1
}			t_bool;

typedef enum e_expand_flag
{
	EXPAND_QUOTE	= 0x00000001,
	EXPAND_VAR		= 0x00000002,
	EXPAND_SPLIT	= 0x00000004
}			t_expand_flag;

typedef enum e_str_type{
	RAW			= 'R',
	S_QUOTE		= '\'',
	D_QUOTE		= '\"'
}			t_str_type;

typedef enum e_status
{
	SUCCESS				= 0,
	E_SYSTEM			= 1,
	E_INVALID_OP		= 2,
	E_NOSET_VAR			= 3,
	E_NUM_ARG_REQ		= 4,
	E_TOO_MANY_ARG		= 5,
	E_INVALID_ID		= 6,
	E_INVALID_OP_ARG	= 7,
	E_SYNTAX			= 8,
	E_NOCOMMAND			= 9,
	E_AMBIGUOUS			= 10,
	E_IS_DIR			= 11,
	E_IS_FILE			= 12,
	E_NO_PATHCOMMAND	= 13,
	E_OPEN				= 14,
	E_GETCWD			= 15,
	E_SIG_INTERRUPT		= 16,
	E_OVER_INT			= 17,
	E_OVER_LIMIT		= 18
}			t_status;

typedef enum e_place
{
	P_SHELL		= 0,
	P_ECHO		= 1,
	P_CD		= 2,
	P_PWD		= 3,
	P_EXPORT	= 4,
	P_UNSET		= 5,
	P_ENV		= 6,
	P_EXIT		= 7
}			t_place;

typedef enum e_vars_type{
	ENV			= 0,
	SHELL		= 1,
	SPECIAL		= 2
}			t_vars_type;

typedef enum e_token_type{
	PIPE			= '|',
	GREATER			= '>',
	LESS			= '<',
	D_GREATER		= 'G',
	D_LESS			= 'L',
	WORD_RAW		= 'R',
	WORD_QUOTE		= 'Q'
}			t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
}				t_token;

typedef struct s_data
{
	t_list		*vars_list[3];
	t_list		*pid_list;
	char		*pwd;
}				t_data;

typedef int				t_exit_status;
typedef t_exit_status	(*t_builtin_func)(t_data *, char **);
typedef t_bool			(*t_file_check_func)(char *);

int						g_received_signal;

// main
t_status		lex_line(char *line, t_token **tokens, int *token_num);
t_status		start_process(t_data *d, t_token *tokens, int start, int end);
t_status		process_pipeline(
					t_data *d, t_token *tokens, int start, int end);
t_status		process_command(t_data *d, t_token *tokens, int start, int end);
t_status		process_redirect(t_token *tokens,
					int i, t_list **save_fd, t_list *vars_list[3]);
t_status		expand_word_token(t_token word_token, t_list *vars_list[3],
					t_expand_flag flag, t_list **expand_list);

// builtins
t_exit_status	mini_echo(t_data *d, char **argv);
t_exit_status	mini_cd(t_data *d, char **argv);
t_exit_status	mini_pwd(t_data *d, char **argv);
t_exit_status	mini_env(t_data *d, char **argv);
t_exit_status	mini_export(t_data *d, char **argv);
t_exit_status	mini_unset(t_data *d, char **argv);
t_exit_status	mini_exit(t_data *d, char **argv);

// var
char			*get_var(t_list *vars_list[3], char *var_name);
t_status		set_var(t_list *vars_list[3], char *var, t_vars_type var_type);
void			delete_var(
					t_list *vars_list[3], char *var_name, t_vars_type var_type);
t_status		set_pwd(t_data *d, t_place place, char *cd_target_dir);
t_status		countup_shlvl_env(t_list **env_list);
void			set_exit_status(
					t_list *special_list, t_exit_status exit_status);
t_list			*get_target_list(t_list *any_list, char *var, int var_name_len);
t_status		add_new_var(t_list **any_list, char *var);

// utils
t_bool			is_redirect_token(t_token token);
t_bool			is_word_token(t_token token);
char			*strjoin_with_null_support(char *s1, char *s2);
char			*strjoin_three(char *s1, char *s2, char *s3);
t_list			*lstnew_with_strdup(char *str);
t_status		convert_strs_to_list(char **strs, t_list **list);
t_status		convert_list_to_strs(t_list *list, char ***strs);
void			delete_last_list(t_list **expand_list);
char			*create_full_path(char *path, char *last_file);
t_status		search_match_path_from_path_var(
					char *last_file, char *path_value,
					t_file_check_func check_func, char **matched_path);
t_bool			check_directory_exist(char *path);
t_bool			check_valid_identifier(char *var, int var_name_len);
void			write_err(char *word,
					t_status status, t_bool is_errno, t_place place);
t_exit_status	get_exit_status_with_errout(
					char *err_word, t_status status, t_place place);
t_status		set_exit_status_with_errout(
					char *err_word, t_status status, t_list *vars_list[3]);
void			free_double_pointer(void **p);
void			free_tokens(t_token *tokens);
t_status		free_and_return(void *p, t_status status);
void			clear_shell_data(t_data *d);

// debug
void			print_line_and_word_start_array(char *line, int *word_start);
void			print_tokens(t_token *tokens, t_list *vars_list[3]);
void			execve_sleep(void);
void			test_vars_list(t_list *vars_list[3]);
t_status		debug_process_command(
					t_token *tokens, int start, int end, t_list *vars_list[3]);

#endif
