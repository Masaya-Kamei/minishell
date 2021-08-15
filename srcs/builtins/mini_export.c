/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:48 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/09 14:01:47 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static t_exit_status	write_exported_vars(t_list *env_list)
{
	int		i;
	char	*equal_pointer;
	char	**envp;

	envp = create_envp(env_list);
	if (envp == NULL)
		return (get_exit_status_with_errout(NULL, E_SYSTEM, P_EXPORT));
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
	free_double_pointer((void **)envp);
	return (0);
}

t_bool	check_valid_identifier(char *var, int var_name_len)
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

t_exit_status	mini_export(t_data *d, char **argv)
{
	int				i;
	t_exit_status	exit_status;
	char			*equal_pointer;
	int				var_name_len;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP, P_EXPORT));
	if (argv[1] == NULL)
		return (write_exported_vars(d->vars_list[ENV]));
	i = 0;
	exit_status = 0;
	while (argv[++i] != NULL)
	{
		equal_pointer = ft_strchr(argv[i], '=');
		if (equal_pointer == NULL)
			var_name_len = ft_strlen(argv[i]);
		else
			var_name_len = equal_pointer - argv[i];
		if (check_valid_identifier(argv[i], var_name_len) == 0)
			exit_status = get_exit_status_with_errout(
					argv[i], E_INVALID_ID, P_EXPORT);
		else if (set_var(d->vars_list, argv[i], ENV) == E_SYSTEM)
			return (get_exit_status_with_errout(NULL, E_SYSTEM, P_EXPORT));
	}
	return (exit_status);
}

// gcc -Wall -Werror -Wextra mini_export.c ../var_env.c ../var_ope.c
//	../var_set_any.c ../var_utils.c ../free.c ../error.c
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
