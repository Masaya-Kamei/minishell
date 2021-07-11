/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:48 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/11 10:40:23 by mkamei           ###   ########.fr       */
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

static int	write_exported_env(t_list *env_list)
{
	int		i;
	char	*equal_pointer;
	char	**envp;

	envp = create_envp_from_env_list(env_list);
	if (envp == NULL)
		return (write_msg(NULL, NULL, ENOMEM, ERRNO));
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

int	check_valid_identifier(char *env, int env_name_len)
{
	int		i;

	if (ft_isalpha(env[0]) == 0 && env[0] != '_')
		return (0);
	i = 0;
	while (i < env_name_len)
	{
		if (ft_isalnum(env[i]) == 0 && env[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	mini_export(char **argv, t_list *env_list)
{
	int			i;
	int			status;
	char		*equal_pointer;
	int			env_name_len;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (write_msg_about_invalid_option(argv[0], argv[1]));
	if (argv[1] == NULL)
		return (write_exported_env(env_list));
	i = 0;
	status = 0;
	while (argv[++i] != NULL)
	{
		equal_pointer = ft_strchr(argv[i], '=');
		if (equal_pointer == NULL)
			env_name_len = ft_strlen(argv[i]);
		else
			env_name_len = equal_pointer - argv[i];
		if (check_valid_identifier(argv[i], env_name_len) == 0)
			status = write_msg_about_invalid_identifier(argv[0], argv[i]);
		else
			if (set_env_in_env_list(env_list, argv[i]) == ENOMEM)
				return (write_msg(NULL, NULL, ENOMEM, ERRNO));
	}
	return (status);
}

// gcc -Wall -Werror -Wextra mini_export.c ../env.c ../env_list.c ../utils.c
//	../write_msg.c -I ../../include -I ../../libft/ ../../libft/libft.a

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
