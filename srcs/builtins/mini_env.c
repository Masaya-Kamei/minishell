/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:37 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/13 12:25:38 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_env_err(char *word, int err_num, t_err_num_type type)
{
	int			i;
	int			status;
	const int	status_table[1][2] = {{ERR_INVALID_OP_ARG, 1}};

	write(2, "minishell: env: ", 16);
	write_err(word, err_num, type);
	if (err_num == ERR_INVALID_OP)
		write(2, "env: usage: env\n", 16);
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

int	mini_env(char **argv, t_list *env_list)
{
	if (argv[1] != NULL)
		return (write_env_err(argv[1], ERR_INVALID_OP_ARG, ORIGINAL));
	env_list = env_list->next;
	while (env_list != NULL)
	{
		ft_putendl_fd(env_list->content, 1);
		env_list = env_list->next;
	}
	return (0);
}

// gcc -Wall -Werror -Wextra mini_env.c ../env.c ../env_list.c ../utils.c
//	../write_err.c -I ../../include -I ../../libft/ ../../libft/libft.a

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*env_list;
// 	int		command_status;

// 	(void)argc;
// 	env_list = create_env_list_from_envp(envp);
// 	argv[0] = "env";
// 	command_status = mini_env(argv, env_list);
// 	set_command_status_env(env_list, command_status);
// 	printf("%s\n", get_env_from_env_list(env_list, "?"));
// 	return (0);
// }
