/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:51 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/09 14:02:24 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit_status	mini_unset(t_data *d, char **argv)
{
	int				i;
	int				var_name_len;
	t_exit_status	exit_status;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP, P_UNSET));
	i = 1;
	exit_status = 0;
	while (argv[i] != NULL)
	{
		var_name_len = ft_strlen(argv[i]);
		if (check_valid_identifier(argv[i], var_name_len) == 0)
			exit_status = get_exit_status_with_errout(
					argv[i], E_INVALID_ID, P_UNSET);
		else
			delete_var(d->vars_list, argv[i], SHELL);
		i++;
	}
	return (exit_status);
}

// gcc -Wall -Werror -Wextra mini_unset.c mini_export.c ../var_env.c
//	../var_ope.c ../var_set_any.c ../var_utils.c ../free.c ../error.c
//	-I ../../include -I ../../libft/ ../../libft/libft.a

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*vars_list[3];
// 	int		exit_status;
// 	int		i;

// 	(void)argc;
// 	vars_list[ENV] = create_env_list(envp);
// 	vars_list[SHELL] = NULL;
// 	vars_list[SPECIAL] = lstnew_with_strdup("?=0  ");
// 	((char *)vars_list[SPECIAL]->content)[3] = '\0';
// 	argv[0] = "unset";
// 	exit_status = mini_unset(argv, vars_list);
// 	set_exit_status(vars_list[SPECIAL], exit_status);
// 	printf("%s\n", get_var(vars_list, "?"));
// 	if (exit_status == 0)
// 	{
// 		i = 1;
// 		while (argv[i] != NULL)
// 		{
// 			printf("%s\n", get_var(vars_list, argv[i]));
// 			i++;
// 		}
// 	}
// 	return (0);
// }
