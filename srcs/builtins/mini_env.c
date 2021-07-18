/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:37 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 21:54:48 by mkamei           ###   ########.fr       */
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
	if (type == ORIGINAL && err_num == ERR_INVALID_OP_ARG)
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

int	mini_env(char **argv, t_list *vars_list[3])
{
	t_list	*current_list;

	if (argv[1] != NULL)
		return (write_env_err(argv[1], ERR_INVALID_OP_ARG, ORIGINAL));
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
//	../var_set_any.c ../var_utils.c ../free.c ../write_err.c
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
