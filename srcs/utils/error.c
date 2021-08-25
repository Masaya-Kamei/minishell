/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 10:48:30 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/25 20:48:29 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_word(char *err_word, t_status status)
{
	if (err_word == NULL || status == E_SYNTAX)
		return ;
	if (status == E_INVALID_ID)
	{
		write(2, "`", 1);
		write(2, err_word, ft_strlen(err_word));
		write(2, "'", 1);
	}
	else if (status == E_INVALID_OP)
		write(2, err_word, 2);
	else
		write(2, err_word, ft_strlen(err_word));
	if (status == E_NOSET_VAR)
		return ;
	write(2, ": ", 2);
}

void	write_err(
	char *err_word, t_status status, t_bool is_errno, t_place place)
{
	const char	commands[8][13] = {"shell-init: "
		, "echo: ", "cd: ", "pwd: ", "export: ", "unset: ", "env: ", "exit: "};
	const char	usages[7][42] = {"", "cd: usage: cd [dir]\n"
		, "pwd: usage: pwd\n", "export: usage: export [name[=value] ...]\n"
		, "unset: usage: unset [name ...]\n", "env: usage: env\n", ""};
	const char	err_msgs[20][36] = {"", "", "invalid option\n", " not set\n"
		, "numeric argument required\n", "too many arguments\n"
		, "not a valid identifier\n", "invalid option or argument\n"
		, "syntax error near unexpected token ", "command not found\n"
		, "ambiguous redirect\n"};

	if (status != E_GETCWD)
		write(2, "minishell: ", 11);
	if (place != P_SHELL || status == E_GETCWD)
		write(2, commands[place], ft_strlen(commands[place]));
	write_word(err_word, status);
	if (is_errno == 0)
		write(2, err_msgs[status], ft_strlen(err_msgs[status]));
	else
		perror(NULL);
	if (status == E_SYNTAX)
		ft_putendl_fd(err_word, 2);
	if (status == E_INVALID_OP)
		write(2, usages[place - 1], ft_strlen(usages[place - 1]));
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
	char *err_word, t_status status, t_place place)
{
	t_exit_status	exit_status;
	const t_bool	is_errno = (
		status == E_SYSTEM || status == E_OPEN || status == E_GETCWD || status == E_OVER_FD || status == E_OVER_LIMIT);
	const int		status_table[8][3][2] = {
		{{E_AMBIGUOUS, 1}, {E_NOCOMMAND, 127}, {E_SYNTAX, 258}}
		, {}
		, {{E_INVALID_OP, 1}, {E_NOSET_VAR, 1}}
		, {{E_INVALID_OP, 1}}
		, {{E_INVALID_ID, 1}, {E_INVALID_OP, 2}}
		, {{E_INVALID_ID, 1}, {E_INVALID_OP, 2}}
		, {{E_INVALID_OP_ARG, 1}}
		, {{E_TOO_MANY_ARG, 1}, {E_NUM_ARG_REQ, 255}}};

	write_err(err_word, status, is_errno, place);
	// if (is_errno == 1 && place == P_SHELL)
	// 	exit_status = errno;
	if (is_errno == 1)
		exit_status = 1;
	else
		exit_status = get_value_from_status_table(
				status, status_table[place]);
	return (exit_status);
}

t_status	set_exit_status_with_errout(
	char *err_word, t_status status, t_list *vars_list[3])
{
	t_exit_status	exit_status;

	exit_status = get_exit_status_with_errout(err_word, status, P_SHELL);
	set_exit_status(vars_list[SPECIAL], exit_status);
	return (SUCCESS);
}
