/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 10:48:30 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/15 12:14:23 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_redirect_fd_err_word(char *err_word)
{
	int		i;
	int		num_len;

	i = 0;
	while (err_word[i] == '0')
		i++;
	num_len = 0;
	while (ft_isdigit(err_word[i + num_len]))
		num_len++;
	if (num_len == 0)
		write(2, &err_word[i - 1], 1);
	else
		write(2, &err_word[i], num_len);
}

static void	write_err_word(char *err_word, t_status status)
{
	if (status == E_SYNTAX)
	{
		write(2, "`", 1);
		if (ft_isdigit(err_word[0]))
			write_redirect_fd_err_word(err_word);
		else
			write(2, err_word, ft_strlen(err_word));
		write(2, "'\n", 2);
	}
	else if (status == E_INVALID_ID)
	{
		write(2, "`", 1);
		write(2, err_word, ft_strlen(err_word));
		write(2, "'", 1);
	}
	else if (status == E_INVALID_OP)
		write(2, err_word, 2);
	else if (status == E_OVER_LIMIT)
		write_redirect_fd_err_word(err_word);
	else
		write(2, err_word, ft_strlen(err_word));
	if (status != E_NOSET_VAR && status != E_SYNTAX)
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
		, "ambiguous redirect\n", "is a directory\n"};

	if (status != E_GETCWD)
		write(2, "minishell: ", 11);
	if (place != P_SHELL || status == E_GETCWD)
		write(2, commands[place], ft_strlen(commands[place]));
	if (err_word && status != E_SYNTAX)
		write_err_word(err_word, status);
	if (is_errno == 0)
		write(2, err_msgs[status], ft_strlen(err_msgs[status]));
	else
		perror(NULL);
	if (status == E_SYNTAX)
		write_err_word(err_word, status);
	if (status == E_INVALID_OP)
		write(2, usages[place - 1], ft_strlen(usages[place - 1]));
}

t_exit_status	get_exit_status_with_errout(
	char *err_word, t_status status, t_place place)
{
	int				i;
	const t_bool	is_errno = (status == E_SYSTEM || status == E_OPEN
		|| status == E_GETCWD || status == E_OVER_INT || status == E_OVER_LIMIT
		|| status == E_NO_PATHCOMMAND || status == E_IS_FILE);
	const int		status_table[8][6][2] = {
		{{E_AMBIGUOUS, 1}, {E_IS_DIR, 126}, {E_IS_FILE, 126}
			, {E_NO_PATHCOMMAND, 127}, {E_NOCOMMAND, 127}, {E_SYNTAX, 258}}
		, {}
		, {{E_INVALID_OP, 1}, {E_NOSET_VAR, 1}}
		, {{E_INVALID_OP, 1}}
		, {{E_INVALID_ID, 1}, {E_INVALID_OP, 2}}
		, {{E_INVALID_ID, 1}, {E_INVALID_OP, 2}}
		, {{E_INVALID_OP_ARG, 1}}
		, {{E_TOO_MANY_ARG, 1}, {E_NUM_ARG_REQ, 255}}};

	write_err(err_word, status, is_errno, place);
	if (is_errno == 1 && status != E_NO_PATHCOMMAND && status != E_IS_FILE)
		return (1);
	else
	{
		i = 0;
		while ((t_status)status_table[place][i][0] != status)
			i++;
		return ((t_exit_status)status_table[place][i][1]);
	}
}

t_status	set_exit_status_with_errout(
	char *err_word, t_status status, t_list *vars_list[3])
{
	t_exit_status	exit_status;

	exit_status = get_exit_status_with_errout(err_word, status, P_SHELL);
	set_exit_status(vars_list[SPECIAL], exit_status);
	return (SUCCESS);
}
