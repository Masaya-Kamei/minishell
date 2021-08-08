/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 10:48:30 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/08 13:08:54 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_word(char *word, t_status status)
{
	if (status == E_GETCWD)
	{
		write(2, "error retrieving current directory: ", 36);
		write(2, "getcwd: cannot access parent directories: ", 42);
		return ;
	}
	if (word == NULL || status == E_SYNTAX)
		return ;
	if (status == E_INVALID_ID)
	{
		write(2, "`", 1);
		write(2, word, ft_strlen(word));
		write(2, "'", 1);
	}
	else if (status == E_INVALID_OP)
		write(2, word, 2);
	else
		write(2, word, ft_strlen(word));
	if (status == E_NOSET_VAR)
		return ;
	write(2, ": ", 2);
}

static void	write_err(
	char *word, t_status status, t_bool is_errno, t_err_place err_place)
{
	const char	commands[8][13] = {"shell-init: "
		, "echo: ", "cd: ", "pwd: ", "export: ", "unset: ", "env: ", "exit: "};
	const char	usages[7][42] = {"", "cd: usage: cd [dir]\n"
		, "pwd: usage: pwd\n", "export: usage: export [name[=value] ...]\n"
		, "unset: usage: unset [name ...]\n", "env: usage: env\n", ""};
	const char	err_msgs[20][36] = {"", "", "invalid option", " not set"
		, "numeric argument required", "too many arguments"
		, "not a valid identifier", "invalid option or argument"
		, "syntax error near unexpected token ", "command not found"
		, "ambiguous redirect", ""};

	if (status != E_GETCWD)
		write(2, "minishell: ", 11);
	if (status == E_GETCWD || err_place != P_SHELL)
		write(2, commands[err_place], ft_strlen(commands[err_place]));
	write_word(word, status);
	if (is_errno == 0)
		write(2, err_msgs[status], ft_strlen(err_msgs[status]));
	else
		write(2, strerror(errno), ft_strlen(strerror(errno)));
	if (status == E_SYNTAX)
		write(2, word, ft_strlen(word));
	write(2, "\n", 1);
	if (status == E_INVALID_OP)
		write(2, usages[err_place - 1], ft_strlen(usages[err_place - 1]));
}

static int	get_value_from_status_table(int key, const int status_table[2][2])
{
	int		i;
	int		value;

	i = 0;
	while (status_table[i][0] != key)
		i++;
	value = status_table[i][1];
	return (value);
}

t_exit_status	get_exit_status_with_errout(
	char *word, t_status status, t_err_place err_place)
{
	t_exit_status	exit_status;
	const t_bool	is_errno = (
		status == E_SYSTEM || status == E_OPEN || status == E_GETCWD
		|| status == E_CHDIR);
	const int		status_table[8][4][2] = {
		{{E_GETCWD, 0}, {E_AMBIGUOUS, 1}, {E_NOCOMMAND, 127}, {E_SYNTAX, 258}}
		, {}
		, {{E_GETCWD, 0}, {E_INVALID_OP, 1}, {E_NOSET_VAR, 1}}
		, {{E_GETCWD, 1}, {E_INVALID_OP, 1}}
		, {{E_INVALID_ID, 1}, {E_INVALID_OP, 2}}
		, {{E_INVALID_ID, 1}, {E_INVALID_OP, 2}}
		, {{E_INVALID_OP_ARG, 1}}
		, {{E_TOO_MANY_ARG, 1}, {E_NUM_ARG_REQ, 255}}};

	write_err(word, status, is_errno, err_place);
	// if (is_errno == 1 && err_place == P_SHELL)
	// 	exit_status = errno;
	if (is_errno == 1 && status != E_GETCWD)
		exit_status = 1;
	else
		exit_status = get_value_from_status_table(
				status, status_table[err_place]);
	return (exit_status);
}
