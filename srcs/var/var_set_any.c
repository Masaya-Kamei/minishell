/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_set_any.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 14:03:48 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/01 14:06:41 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_current_absolute_path(void)
{
	int		size;
	char	*absolute_path;

	size = 128;
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

t_status	set_pwd_var(t_list *vars_list[3], t_bool init)
{
	char		*var;
	char		*value;
	int			var_type;
	t_status	status;

	if (init == 1)
		var_type = ENV;
	else
		var_type = SHELL;
	value = get_current_absolute_path();
	if (value == NULL)
		return (E_SYSTEM);
	var = ft_strjoin("PWD=", value);
	free(value);
	if (var == NULL)
		return (E_SYSTEM);
	status = set_var(vars_list, var, var_type);
	free(var);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	return (SUCCESS);
}

t_status	set_oldpwd_var(t_list *vars_list[3], t_bool init)
{
	t_list		*target_list;
	char		*var;
	char		*value;
	t_status	status;

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
		return (E_SYSTEM);
	status = set_var(vars_list, var, SHELL);
	free(var);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	return (SUCCESS);
}

void	set_exit_status(t_list *special_list, t_exit_status exit_status)
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

t_status	countup_shlvl_env(t_list **env_list)
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
		return (E_SYSTEM);
	free(target_list->content);
	target_list->content = ft_strjoin("SHLVL=", value);
	free(value);
	if (target_list->content == NULL)
		return (E_SYSTEM);
	return (SUCCESS);
}
