/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:12:35 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/16 13:35:51 by mkamei           ###   ########.fr       */
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

// gcc -Wall -Werror -Wextra mini_exit.c ../var_env.c ../var_ope.c
//	../var_set_any.c ../var_utils.c ../free.c ../error.c
//	-I ../../include -I ../../libft/ ../../libft/libft.a

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*vars_list[3];
// 	int		exit_status;

// 	(void)argc;
// 	vars_list[ENV] = create_env_list(envp);
// 	vars_list[SHELL] = NULL;
// 	vars_list[SPECIAL] = lstnew_with_strdup("?=0  ");
// 	((char *)vars_list[SPECIAL]->content)[3] = '\0';
// 	argv[0] = "exit";
// 	exit_status = mini_exit(argv, NULL);
// 	set_exit_status(vars_list[SPECIAL], exit_status);
// 	printf("%s\n", get_var(vars_list, "?"));
// 	return (0);
// }
