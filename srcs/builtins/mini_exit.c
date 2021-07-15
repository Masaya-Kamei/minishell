/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:12:35 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 16:07:39 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_exit_err(char *word, int err_num, t_err_num_type type)
{
	int			i;
	int			status;
	const int	status_table[2][2] = {
		{ERR_NUM_ARG_REQ, 255}, {ERR_TOO_MANY_ARG, 1}};

	write(2, "minishell: exit: ", 17);
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

int	mini_exit(char **argv)
{
	int		exit_num;
	int		i;

	write(2, "exit\n", 5);
	if (argv[1] == NULL)
		exit(0);
	i = 0;
	if (argv[1][i] == '+' || argv[1][i] == '-')
		i++;
	exit_num = 0;
	while (argv[1][i] != '\0')
	{
		if (ft_isdigit(argv[1][i]) == 0)
			exit(write_exit_err(argv[1], ERR_NUM_ARG_REQ, ORIGINAL));
		exit_num = exit_num * 10 + argv[1][i] - '0';
		i++;
	}
	if (argv[2] != NULL)
		return (write_exit_err(NULL, ERR_TOO_MANY_ARG, ORIGINAL));
	exit_num = exit_num % 256;
	if (argv[1][0] == '-')
		exit_num = 256 - exit_num;
	exit(exit_num);
}
	// printf("%d\n", exit_num);
