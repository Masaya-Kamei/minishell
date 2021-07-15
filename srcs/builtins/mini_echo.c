/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:12:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 17:44:33 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_echo(char **argv)
{
	int		i;
	int		option_n;

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

// gcc -Wall -Werror -Wextra mini_echo.c ../write_err.c
//	-I ../../include -I ../../libft/ ../../libft/libft.a

// int	main(int argc, char **argv)
// {
// 	int		exit_status;

// 	(void)argc;
// 	argv[0] = "echo";
// 	exit_status = mini_echo(argv);
// 	printf("%d\n", exit_status);
// 	return (0);
// }
