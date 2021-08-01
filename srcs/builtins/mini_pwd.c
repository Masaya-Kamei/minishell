/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:11:20 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/01 12:49:16 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit_status	mini_pwd(char **argv, t_list *vars_list[3])
{
	char	*absolute_path;

	(void)(**vars_list);
	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP, P_PWD));
	absolute_path = get_current_absolute_path();
	if (absolute_path == NULL)
		return (get_exit_status_with_errout(NULL, E_SYSTEM, P_PWD));
	ft_putendl_fd(absolute_path, 1);
	free(absolute_path);
	return (0);
}

// gcc -Wall -Werror -Wextra mini_pwd.c ../var_env.c ../var_ope.c
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
// 	argv[0] = "pwd";
// 	exit_status = mini_pwd(argv, NULL);
// 	set_exit_status(vars_list[SPECIAL], exit_status);
// 	printf("%s\n", get_var(vars_list, "?"));
// 	return (0);
// }
