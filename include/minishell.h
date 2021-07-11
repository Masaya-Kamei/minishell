/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 09:39:27 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/11 10:33:51 by mkamei           ###   ########.fr       */
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

# define SUCCESS 0
# define INVALID_OP_ARG 1

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
int		expand_word_token(char **word, t_list *env_list);
int		mini_env(char **argv, t_list *env_list);
int		mini_export(char **argv, t_list *env_list);
int		mini_unset(char **argv, t_list *env_list);
int		check_valid_identifier(char *env, int env_name_len);
t_list	*lstnew_with_strdup(char *str);
t_list	*create_env_list_from_envp(char **envp);
char	**create_envp_from_env_list(t_list *env_list);
void	set_command_status_env(t_list *env_list, int command_status);
char	*get_env_from_env_list(t_list *env_list, char *env_name);
int		set_env_in_env_list(t_list *env_list, char *env);
int		delete_oldpwd_env_value(t_list *env_list);
int		countup_shlvl_env(t_list *env_list);
int		write_msg(char *command, char *word, int err_num, t_err_num_type type);
int		write_msg_about_invalid_identifier(char *command, char *word);
int		write_msg_about_invalid_option(char *command, char *word);
void	free_double_pointer(char **strs);
void	free_tokens(t_token *tokens);
int		free_and_return(void *p, int status);
void	free_and_fill_null(char **p);
void	print_line_and_word_start_array(char *line, int *word_start);
void	print_tokens(t_token *tokens, t_list *env_list);
void	execve_sleep(void);

int		g_received_signal;

#endif
