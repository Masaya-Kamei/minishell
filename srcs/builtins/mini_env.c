/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:37 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/20 19:01:11 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit_status	mini_env(char **argv, t_list *vars_list[3])
{
	t_list	*current_list;

	if (argv[1] != NULL)
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP_ARG, P_ENV));
	current_list = vars_list[ENV];
	while (current_list != NULL)
	{
		if (ft_strchr(current_list->content, '=') != NULL)
			ft_putendl_fd(current_list->content, 1);
		current_list = current_list->next;
	}
	return (0);
}

// gcc -Wall -Werror -Wextra mini_env.c ../var_env.c ../var_ope.c
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
// 	argv[0] = "env";
// 	exit_status = mini_env(argv, vars_list);
// 	set_exit_status(vars_list[SPECIAL], exit_status);
// 	printf("%s\n", get_var(vars_list, "?"));
// 	return (0);
// }
