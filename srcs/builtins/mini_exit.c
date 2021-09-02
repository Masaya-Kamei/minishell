/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:12:35 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/02 14:12:30 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit_status	mini_exit(t_data *d, char **argv)
{
	unsigned long long	exit_num;
	int					i;

	write(2, "exit\n", 5);
	if (argv[1] == NULL)
		exit(ft_atoi(get_var(d->vars_list, "?")));
	i = (argv[1][0] == '+' || argv[1][0] == '-');
	if (argv[1][i] == '\0')
		exit(get_exit_status_with_errout(argv[1], E_NUM_ARG_REQ, P_EXIT));
	exit_num = 0;
	while (argv[1][i] != '\0')
	{
		exit_num = exit_num * 10 + argv[1][i] - '0';
		if (ft_isdigit(argv[1][i]) == 0
			|| (argv[1][0] == '-' && exit_num > (unsigned long long) -LLONG_MIN)
			|| (argv[1][0] != '-' && exit_num > (unsigned long long)LLONG_MAX))
			exit(get_exit_status_with_errout(argv[1], E_NUM_ARG_REQ, P_EXIT));
		i++;
	}
	if (argv[2] != NULL)
		return (get_exit_status_with_errout(NULL, E_TOO_MANY_ARG, P_EXIT));
	exit_num = exit_num % 256;
	if (argv[1][0] == '-')
		exit_num = 256 - exit_num;
	exit(exit_num);
}
