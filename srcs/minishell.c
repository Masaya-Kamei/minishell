/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:30:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/31 14:45:48 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	loop_minishell(void)
{
	char	*line;
	char	**argv;
	int		argc;
	int		status;

	while (1)
	{
		write(1, "minishell$ ", 11);
		status = get_line(&line);
		if (status == ERROR)
			exit(1);
		status = split_line_into_words(line, &argv, &argc);
		if (status == ERROR)
			exit(free_and_return(line, 1));
		free(line);
		// print_argv(argv);
		// process_commandline(argv, 0, argc - 1);
		free_double_pointer(argv);
	}
}

int	main(void)
{
	loop_minishell();
	return (0);
}
