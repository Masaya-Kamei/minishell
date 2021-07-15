/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:10:11 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 21:57:57 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_cd_err(char *word, int err_num, t_err_num_type type)
{
	int			i;
	int			status;
	const int	status_table[3][2] = {
		{ERR_INVALID_OP, 1}, {ERR_NOSET_OLDPWD, 1}, {ERR_NOSET_HOME, 1}};

	write(2, "minishell: cd: ", 15);
	write_err(word, err_num, type);
	if (type == ORIGINAL && err_num == ERR_INVALID_OP)
		write(2, "cd: usage: cd [dir]\n", 20);
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

int	mini_cd(char **argv, t_list *vars_list[3])
{
	char	*target_dir;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (write_cd_err(argv[1], ERR_INVALID_OP, ORIGINAL));
	if (argv[1] == NULL)
	{
		target_dir = get_var(vars_list, "HOME");
		if (target_dir == NULL)
			return (write_cd_err(NULL, ERR_NOSET_HOME, ORIGINAL));
	}
	else if (argv[1][0] == '-' && argv[1][1] == '\0')
	{
		target_dir = get_var(vars_list, "OLDPWD");
		if (target_dir == NULL)
			return (write_cd_err(NULL, ERR_NOSET_OLDPWD, ORIGINAL));
	}
	else
		target_dir = argv[1];
	if (chdir(target_dir) == -1)
		return (write_cd_err(argv[1], errno, ERRNO));
	if (set_oldpwd_var(vars_list, 0) == ERR_MALLOC
		|| set_pwd_var(vars_list, 0) == ERR_MALLOC)
		return (write_shell_err(NULL, ERR_MALLOC, ORIGINAL));
	return (0);
}

// gcc -Wall -Werror -Wextra mini_cd.c ../var_env.c ../var_ope.c
//	../var_set_any.c ../var_utils.c ../free.c ../write_err.c
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
// 	return (0);
// }