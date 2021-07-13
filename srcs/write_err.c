/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 10:48:30 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/13 15:42:35 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
# define SUCCESS 0
# define ERR_MALLOC 1
# define ERR_INVALID_OP 2
# define ERR_NOSET_HOME 3
# define ERR_NOSET_OLDPWD 4
# define ERR_NUM_ARG_REQ 5
# define ERR_TOO_MANY_ARG 6
# define ERR_INVALID_ID 7
# define ERR_INVALID_OP_ARG 8
void	write_err(char *word, int err_num, t_err_num_type type)
{
	const char	err_msgs[10][30] = {
		"", "Cannot allocate memory", "invalid option", "HOME not set"
		, "OLDPWD not set", "numeric argument required", "too many arguments"
		, "not a valid identifier", "invalid option or argument"};

	if (word != NULL)
	{
		if (err_num == ERR_INVALID_ID)
			write(2, "`", 1);
		if (err_num == ERR_INVALID_OP)
			write(2, word, 2);
		else
			write(2, word, ft_strlen(word));
		if (err_num == ERR_INVALID_ID)
			write(2, "'", 1);
		write(2, ": ", 2);
	}
	if (type == ERRNO)
		write(2, strerror(err_num), ft_strlen(strerror(err_num)));
	else
		write(2, err_msgs[err_num], ft_strlen(err_msgs[err_num]));
	write(2, "\n", 1);
}

int	write_shell_err(char *word, int err_num, t_err_num_type type)
{
	int			i;
	int			status;
	const int	status_table[10][2] = {{ERR_MALLOC, 1}};

	write(2, "minishell: ", 11);
	write_err(word, err_num, type);
	if (type == ERRNO)
		status = 1;
	else
	{
		i = 0;
		while (status_table[i][0] != err_num)
			i++;
		status = status_table[i][1];
	}
	return (status);
}
