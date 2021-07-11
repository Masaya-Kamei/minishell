/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:37 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/11 10:40:58 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_env(char **argv, t_list *env_list)
{
	if (argv[1] != NULL)
	{
		write_msg(argv[0], argv[1], INVALID_OP_ARG, ORIGINAL);
		write(2, "env: usage: env\n", 16);
		return (1);
	}
	env_list = env_list->next;
	while (env_list != NULL)
	{
		ft_putendl_fd(env_list->content, 1);
		env_list = env_list->next;
	}
	return (0);
}

// gcc -Wall -Werror -Wextra mini_env.c ../env.c ../env_list.c ../utils.c
//	../write_msg.c -I ../../include -I ../../libft/ ../../libft/libft.a

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
