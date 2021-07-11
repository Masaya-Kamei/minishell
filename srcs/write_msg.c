/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 10:48:30 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/11 10:26:53 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	write_msg(char *command, char *word, int err_num, t_err_num_type type)
{
	const char	err_msg[10][30] = {"", "invalid option or argument"};

	write(2, "minishell: ", 11);
	if (command != NULL)
	{
		write(2, command, ft_strlen(command));
		write(2, ": ", 2);
	}
	if (word != NULL)
	{
		write(2, word, ft_strlen(word));
		write(2, ": ", 2);
	}
	if (type == ERRNO)
		write(2, strerror(err_num), ft_strlen(strerror(err_num)));
	else
		write(2, err_msg[err_num], ft_strlen(err_msg[err_num]));
	write(2, "\n", 1);
	return (err_num);
}

int	write_msg_about_invalid_identifier(char *command, char *word)
{
	const int	identifier_error_status = 1;

	write(2, "minishell: ", 11);
	write(2, command, ft_strlen(command));
	write(2, ": ", 2);
	write(2, "`", 1);
	write(2, word, ft_strlen(word));
	write(2, "': ", 3);
	write(2, "not a valid identifier\n", 23);
	return (identifier_error_status);
}

int	write_msg_about_invalid_option(char *command, char *word)
{
	int		option_error_status;

	write(2, "minishell: ", 11);
	write(2, command, ft_strlen(command));
	write(2, ": ", 2);
	write(2, word, 2);
	write(2, ": ", 2);
	write(2, "invalid option\n", 15);
	option_error_status = 1;
	if (ft_strncmp(command, "cd", 3) == 0)
		write(2, "cd: usage: cd [dir]\n", 20);
	else if (ft_strncmp(command, "pwd", 4) == 0)
		write(2, "pwd: usage: pwd\n", 16);
	else if (ft_strncmp(command, "export", 7) == 0)
	{
		option_error_status = 2;
		write(2, "export: usage: export [name[=value] ...]\n", 41);
	}
	else if (ft_strncmp(command, "unset", 6) == 0)
	{
		option_error_status = 2;
		write(2, "unset: usage: unset [name ...]\n", 31);
	}
	return (option_error_status);
}
