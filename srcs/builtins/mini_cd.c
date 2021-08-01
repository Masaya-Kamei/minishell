/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:10:11 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/01 12:48:06 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit_status	mini_cd(char **argv, t_list *vars_list[3])
{
	char	*target_dir;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP, P_CD));
	if (argv[1] == NULL)
	{
		target_dir = get_var(vars_list, "HOME");
		if (target_dir == NULL)
			return (get_exit_status_with_errout("HOME", E_NOSET_VAR, P_CD));
	}
	else if (argv[1][0] == '-' && argv[1][1] == '\0')
	{
		target_dir = get_var(vars_list, "OLDPWD");
		if (target_dir == NULL)
			return (get_exit_status_with_errout("OLDPWD", E_NOSET_VAR, P_CD));
	}
	else
		target_dir = argv[1];
	if (chdir(target_dir) == -1)
		return (get_exit_status_with_errout(target_dir, E_SYSTEM, P_CD));
	if (set_oldpwd_var(vars_list, 0) == E_SYSTEM
		|| set_pwd_var(vars_list, 0) == E_SYSTEM)
		return (get_exit_status_with_errout(NULL, E_SYSTEM, P_CD));
	return (0);
}

// gcc -Wall -Werror -Wextra mini_cd.c mini_unset.c mini_export.c ../var_env.c
//	../var_ope.c ../var_set_any.c ../var_utils.c ../free.c ../error.c
//	-I ../../include -I ../../libft/ ../../libft/libft.a

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*vars_list[3];
// 	int		exit_status;
// 	char	path[100];

// 	(void)argc;
// 	vars_list[ENV] = create_env_list(envp);
// 	vars_list[SHELL] = NULL;
// 	vars_list[SPECIAL] = lstnew_with_strdup("?=0  ");
// 	((char *)vars_list[SPECIAL]->content)[3] = '\0';
// 	argv[0] = "cd";
// 	exit_status = mini_cd(argv, vars_list);
// 	set_exit_status(vars_list[SPECIAL], exit_status);
// 	printf("%s\n", get_var(vars_list, "?"));
// 	if (exit_status == 0)
// 	{
// 		getcwd(path, 100);
// 		write(1, path, ft_strlen(path));
// 		write(1, "\n", 1);
// 		printf("PWD=%s\n", get_var(vars_list, "PWD"));
// 		printf("OLDPWD=%s\n", get_var(vars_list, "OLDPWD"));
// 	}
// 	argv[0] = "unset";
// 	argv[1] = "PWD";
// 	argv[2] = "OLDPWD";
// 	argv[3] = NULL;
// 	exit_status = mini_unset(argv, vars_list);
// 	printf("%d\n", exit_status);
// 	argv[0] = "cd";
// 	argv[1] = NULL;
// 	exit_status = mini_cd(argv, vars_list);
// 	printf("%d\n", exit_status);
// 	printf("OLDPWD=%s\n", get_var(vars_list, "OLDPWD"));
// 	printf("PWD=%s\n", get_var(vars_list, "PWD"));
// 	printf("%s\n", (char *)vars_list[SHELL]->content);
// 	printf("%s\n", (char *)vars_list[SHELL]->next->content);
// 	return (0);
// }
