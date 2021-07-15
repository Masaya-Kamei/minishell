/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_set_any.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 14:03:48 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 13:22:49 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_current_absolute_path(void)
{
	int		size;
	char	*absolute_path;

	size = 10;
	absolute_path = NULL;
	while (absolute_path == NULL)
	{
		absolute_path = (char *)malloc(sizeof(char) * (size + 1));
		if (absolute_path == NULL)
			return (NULL);
		absolute_path = getcwd(absolute_path, size);
		if (absolute_path == NULL)
		{
			free(absolute_path);
			size *= 2;
		}
	}
	return (absolute_path);
}

int	set_pwd_var(t_list *vars_list[3], int init)
{
	char	*var;
	char	*value;
	int		var_type;
	int		status;

	if (init == 1)
		var_type = ENV;
	else
		var_type = SHELL;
	value = get_current_absolute_path();
	if (value == NULL)
		return (ERR_MALLOC);
	var = ft_strjoin("PWD=", value);
	free(value);
	if (var == NULL)
		return (ERR_MALLOC);
	status = set_var(vars_list, var, var_type);
	free(var);
	if (status == ERR_MALLOC)
		return (ERR_MALLOC);
	return (SUCCESS);
}

int	set_oldpwd_var(t_list *vars_list[3], int init)
{
	t_list	*target_list;
	char	*var;
	char	*value;
	int		status;

	if (init == 1)
	{
		target_list = get_target_list(vars_list[ENV], "OLDPWD", 6);
		if (target_list == NULL)
			return (add_new_var(&vars_list[ENV], "OLDPWD"));
		((char *)target_list->content)[6] = '\0';
		return (SUCCESS);
	}
	value = get_var(vars_list, "PWD");
	if (value == NULL)
		var = ft_strdup("OLDPWD=");
	else
		var = ft_strjoin("OLDPWD=", value);
	if (var == NULL)
		return (ERR_MALLOC);
	status = set_var(vars_list, var, SHELL);
	free(var);
	if (status == ERR_MALLOC)
		return (ERR_MALLOC);
	return (SUCCESS);
}

void	set_exit_status(t_list *special_list, int exit_status)
{
	int		digit_num;
	t_list	*target_list;

	if (exit_status / 100 > 0)
		digit_num = 3;
	else if (exit_status / 10 > 0)
		digit_num = 2;
	else
		digit_num = 1;
	target_list = get_target_list(special_list, "?", 1);
	((char *)target_list->content)[2 + digit_num] = '\0';
	digit_num--;
	while (digit_num >= 0)
	{
		((char *)target_list->content)[2 + digit_num] = exit_status % 10 + '0';
		exit_status /= 10;
		digit_num--;
	}
}

int	countup_shlvl_env(t_list **env_list)
{
	char	*value;
	t_list	*target_list;

	target_list = get_target_list(*env_list, "SHLVL", 5);
	if (target_list == NULL)
		return (add_new_var(env_list, "SHLVL=1"));
	if (((char *)target_list->content)[5] == '\0')
		value = ft_itoa(1);
	else
		value = ft_itoa(ft_atoi(&((char *)target_list->content)[6]) + 1);
	if (value == NULL)
		return (ERR_MALLOC);
	free(target_list->content);
	target_list->content = ft_strjoin("SHLVL=", value);
	free(value);
	if (target_list->content == NULL)
		return (ERR_MALLOC);
	return (SUCCESS);
}

// gcc -Wall -Werror -Wextra env.c env_list.c utils.c write_err.c
//		builtins/mini_env.c -I ../include -I ../libft/ ../libft/libft.a

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*env_list;
// 	int		i;

// 	(void)argc;
// 	i = 0;
// 	while (envp[i] != NULL)
// 		ft_putendl_fd(envp[i++], 1);
// 	write(1, "\n", 1);
// 	env_list = create_env_list_from_envp(envp);
// 	countup_shlvl_env(env_list);
// 	delete_oldpwd_env_value(env_list);
// 	argv[0] = "env";
// 	mini_env(argv, env_list);
// 	printf("\n%s\n", get_env_from_env_list(env_list, "?"));
// 	set_command_status_env(env_list, 127);
// 	printf("%s\n", get_env_from_env_list(env_list, "?"));
// 	printf("%s\n", get_env_from_env_list(env_list, "SHELL"));
// 	set_env_in_env_list(env_list, "AAA=a");
// 	printf("%s\n", get_env_from_env_list(env_list, "AAA"));
// 	set_env_in_env_list(env_list, "AAA");
// 	printf("%s\n", get_env_from_env_list(env_list, "AAA"));
// 	set_env_in_env_list(env_list, "BBB");
// 	printf("%s\n", get_env_from_env_list(env_list, "BBB"));
// 	set_env_in_env_list(env_list, "BBB=");
// 	printf("%s\n", get_env_from_env_list(env_list, "BBB"));
// 	set_env_in_env_list(env_list, "BBB=b");
// 	printf("%s\n\n", get_env_from_env_list(env_list, "BBB"));
// 	envp = create_envp_from_env_list(env_list);
// 	i = 0;
// 	while (envp[i] != NULL)
// 		ft_putendl_fd(envp[i++], 1);
// 	return (0);
// }
