/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:11:20 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 22:08:29 by mkamei           ###   ########.fr       */
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
	if (type == ORIGINAL && err_num == ERR_INVALID_OP)
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

int	mini_pwd(char **argv, t_list *vars_list[3])
{
	char	*absolute_path;

	(void)(**vars_list);
	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (write_pwd_err(argv[1], ERR_INVALID_OP, ORIGINAL));
	absolute_path = get_current_absolute_path();
	if (absolute_path == NULL)
		return (write_shell_err(NULL, ERR_MALLOC, ORIGINAL));
	ft_putendl_fd(absolute_path, 1);
	free(absolute_path);
	return (0);
}

// gcc -Wall -Werror -Wextra mini_pwd.c ../var_env.c ../var_ope.c
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
// 	argv[0] = "pwd";
// 	exit_status = mini_pwd(argv, NULL);
// 	set_exit_status(vars_list[SPECIAL], exit_status);
// 	printf("%s\n", get_var(vars_list, "?"));
// 	return (0);
// }
