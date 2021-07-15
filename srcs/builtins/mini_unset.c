/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:51 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 12:10:04 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_unset_err(char *word, int err_num, t_err_num_type type)
{
	int			i;
	int			status;
	const int	status_table[2][2] = {{ERR_INVALID_OP, 2}, {ERR_INVALID_ID, 1}};

	write(2, "minishell: unset: ", 18);
	write_err(word, err_num, type);
	if (err_num == ERR_INVALID_OP)
		write(2, "unset: usage: unset [name ...]\n", 31);
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

int	mini_unset(char **argv, t_list *vars_list[3])
{
	int		i;
	int		var_name_len;
	int		status;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (write_unset_err(argv[1], ERR_INVALID_OP, ORIGINAL));
	i = 1;
	status = 0;
	while (argv[i] != NULL)
	{
		var_name_len = ft_strlen(argv[i]);
		if (check_valid_identifier(argv[i], var_name_len) == 0)
			status = write_unset_err(argv[i], ERR_INVALID_ID, ORIGINAL);
		else
			delete_var(vars_list, argv[i], SHELL);
		i++;
	}
	return (status);
}

// gcc -Wall -Werror -Wextra mini_unset.c minit_export.c ../env.c ../env_list.c
// ../utils.c ../write_err.c -I ../../include -I ../../libft/ ../../libft/libft.a

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*env_list;
// 	int		command_status;

// 	(void)argc;
// 	env_list = create_env_list_from_envp(envp);
// 	argv[0] = "unset";
// 	command_status = mini_unset(argv, env_list);
// 	set_command_status_env(env_list, command_status);
// 	printf("%s\n", get_env_from_env_list(env_list, "?"));
// 	if (command_status == 0)
// 		printf("%s\n", get_env_from_env_list(env_list, argv[1]));
// 	return (0);
// }
