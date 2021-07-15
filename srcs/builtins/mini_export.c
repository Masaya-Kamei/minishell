/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:48 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 13:23:35 by mkamei           ###   ########.fr       */
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
	if (err_num == ERR_INVALID_OP)
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

// gcc -Wall -Werror -Wextra mini_export.c ../env.c ../env_list.c ../utils.c
//	../write_err.c -I ../../include -I ../../libft/ ../../libft/libft.a

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*env_list;
// 	int		command_status;
// 	char	*equal_pointer;

// 	(void)argc;
// 	env_list = create_env_list_from_envp(envp);
// 	argv[0] = "export";
// 	command_status = mini_export(argv, env_list);
// 	set_command_status_env(env_list, command_status);
// 	printf("%s\n", get_env_from_env_list(env_list, "?"));
// 	if (command_status == 0)
// 	{
// 		equal_pointer = ft_strchr(argv[1], '=');
// 		if (equal_pointer != NULL)
// 			*equal_pointer = '\0';
// 		printf("%s\n", get_env_from_env_list(env_list, argv[1]));
// 	}
// 	return (0);
// }
