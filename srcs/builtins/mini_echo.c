/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:12:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/09 14:00:42 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit_status	mini_echo(t_data *d, char **argv)
{
	int		i;
	int		option_n;

	(void)*d;
	i = 1;
	option_n = 0;
	while (argv[i] != NULL && ft_strncmp(argv[i], "-n", 3) == 0)
	{
		option_n = 1;
		i++;
	}
	if (argv[i] != NULL)
	{
		write(1, argv[i], ft_strlen(argv[i]));
		i++;
	}
	while (argv[i] != NULL)
	{
		write(1, " ", 1);
		write(1, argv[i], ft_strlen(argv[i]));
		i++;
	}
	if (option_n == 0)
		write(1, "\n", 1);
	return (0);
}

// gcc -Wall -Werror -Wextra mini_echo.c ../error.c
//	-I ../../include -I ../../libft/ ../../libft/libft.a

// int	main(int argc, char **argv)
// {
// 	int		exit_status;

// 	(void)argc;
// 	argv[0] = "echo";
// 	exit_status = mini_echo(argv, NULL);
// 	printf("%d\n", exit_status);
// 	return (0);
// }
