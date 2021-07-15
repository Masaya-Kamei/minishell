/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:48 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 18:33:05 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_export_err(char *word, int err_num, t_err_num_type type)
{
	int			i;
	int			status;
	const int	status_table[2][2] = {{ERR_INVALID_OP, 2}, {ERR_INVALID_ID, 1}};

	write(2, "minishell: export: ", 19);
	write_err(word, err_num, type);
	if (type == ORIGINAL && err_num == ERR_INVALID_OP)
		write(2, "export: usage: export [name[=value] ...]\n", 41);
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

static void	bubble_sort_envp(char **envp)
{
	int		start;
	int		end;
	int		i;
	char	*tmp;

	end = 0;
	while (envp[end] != NULL)
		end++;
	end--;
	start = 0;
	while (start < end)
	{
		i = end - 1;
		while (start <= i)
		{
			if (ft_strncmp(envp[i], envp[i + 1], ft_strlen(envp[i]) + 1) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[i + 1];
				envp[i + 1] = tmp;
			}
			i--;
		}
		start++;
	}
}

static int	write_exported_vars(t_list *env_list)
{
	int		i;
	char	*equal_pointer;
	char	**envp;

	envp = create_envp(env_list);
	if (envp == NULL)
		return (write_shell_err(NULL, ERR_MALLOC, ORIGINAL));
	bubble_sort_envp(envp);
	i = -1;
	while (envp[++i] != NULL)
	{
		write(1, "declare -x ", 11);
		equal_pointer = ft_strchr(envp[i], '=');
		if (equal_pointer == NULL)
			ft_putendl_fd(envp[i], 1);
		else
		{
			write(1, envp[i], equal_pointer - envp[i]);
			write(1, "=\"", 2);
			ft_putstr_fd(equal_pointer + 1, 1);
			write(1, "\"\n", 2);
		}
	}
	free_double_pointer(envp);
	return (0);
}

int	check_valid_identifier(char *var, int var_name_len)
{
	int		i;

	if (ft_isalpha(var[0]) == 0 && var[0] != '_')
		return (0);
	i = 0;
	while (i < var_name_len)
	{
		if (ft_isalnum(var[i]) == 0 && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	mini_export(char **argv, t_list *vars_list[3])
{
	int			i;
	int			status;
	char		*equal_pointer;
	int			var_name_len;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (write_export_err(argv[1], ERR_INVALID_OP, ORIGINAL));
	if (argv[1] == NULL)
		return (write_exported_vars(vars_list[ENV]));
	i = 0;
	status = 0;
	while (argv[++i] != NULL)
	{
		equal_pointer = ft_strchr(argv[i], '=');
		if (equal_pointer == NULL)
			var_name_len = ft_strlen(argv[i]);
		else
			var_name_len = equal_pointer - argv[i];
		if (check_valid_identifier(argv[i], var_name_len) == 0)
			status = write_export_err(argv[i], ERR_INVALID_ID, ORIGINAL);
		else
			if (set_var(vars_list, argv[i], ENV) == ERR_MALLOC)
				return (write_shell_err(NULL, ERR_MALLOC, ORIGINAL));
	}
	return (status);
}

// gcc -Wall -Werror -Wextra mini_export.c ../var_env.c ../var_ope.c
//	../var_set_any.c ../var_utils.c ../free.c ../write_err.c
//	-I ../../include -I ../../libft/ ../../libft/libft.a

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*vars_list[3];
// 	int		exit_status;
// 	int		i;
// 	char	*equal_pointer;

// 	(void)argc;
// 	vars_list[ENV] = create_env_list(envp);
// 	vars_list[SHELL] = NULL;
// 	vars_list[SPECIAL] = lstnew_with_strdup("?=0  ");
// 	((char *)vars_list[SPECIAL]->content)[3] = '\0';
// 	argv[0] = "export";
// 	exit_status = mini_export(argv, vars_list);
// 	set_exit_status(vars_list[SPECIAL], exit_status);
// 	printf("%s\n", get_var(vars_list, "?"));
// 	if (exit_status == 0)
// 	{
// 		i = 1;
// 		while (argv[i] != NULL)
// 		{
// 			equal_pointer = ft_strchr(argv[i], '=');
// 			if (equal_pointer != NULL)
// 				*equal_pointer = '\0';
// 			printf("%s\n", get_var(vars_list, argv[i]));
// 			i++;
// 		}
// 	}
// 	return (0);
// }