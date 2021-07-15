/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:11:20 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 16:53:00 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_pwd_err(char *word, int err_num, t_err_num_type type)
{
	int			i;
	int			status;
	const int	status_table[1][2] = {{ERR_INVALID_OP, 1}};

	write(2, "minishell: pwd: ", 16);
	write_err(word, err_num, type);
	if (err_num == ERR_INVALID_OP)
		write(2, "pwd: usage: pwd\n", 16);
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

int	mini_pwd(char **argv)
{
	char	*absolute_path;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (write_pwd_err(argv[1], ERR_INVALID_OP, ORIGINAL));
	absolute_path = get_current_absolute_path();
	if (absolute_path == NULL)
		return (write_shell_err(NULL, ERR_MALLOC, ORIGINAL));
	ft_putendl_fd(absolute_path, 1);
	free(absolute_path);
	return (0);
}
