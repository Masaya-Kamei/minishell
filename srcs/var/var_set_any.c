/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_set_any.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 14:03:48 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/14 17:48:41 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_status	set_pwd_var(t_data *d, t_place place)
{
	char		*var;
	t_status	status;
	t_vars_type	var_type;

	if (place == P_SHELL)
		var_type = ENV;
	else
		var_type = SHELL;
	var = ft_strjoin("PWD=", d->pwd);
	if (var == NULL)
		return (E_SYSTEM);
	status = set_var(d->vars_list, var, var_type);
	free(var);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	set_oldpwd_var(t_list *vars_list[3], t_place place)
{
	t_list		*target_list;
	char		*var;
	char		*value;
	t_status	status;

	if (place == P_SHELL)
	{
		target_list = get_target_list(vars_list[ENV], "OLDPWD", 6);
		if (target_list == NULL)
			return (add_new_var(&vars_list[ENV], "OLDPWD"));
		((char *)target_list->content)[6] = '\0';
		return (SUCCESS);
	}
	value = get_var(vars_list, "PWD");
	var = strjoin_with_null_support("OLDPWD=", value);
	if (var == NULL)
		return (E_SYSTEM);
	status = set_var(vars_list, var, SHELL);
	free(var);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	return (SUCCESS);
}

t_status	set_pwd(t_data *d, t_place place, char *cd_target_dir)
{
	char	*new_pwd;

	if (place == P_CD && ft_strncmp(cd_target_dir, "//", 3) == 0)
		new_pwd = ft_strdup("//");
	else
	{
		new_pwd = getcwd(NULL, 0);
		if (new_pwd == NULL)
		{
			write_err(GETCWD_EMSG, E_GETCWD, 1, place);
			if (place == P_SHELL)
				return (SUCCESS);
			new_pwd = create_full_path(d->pwd, cd_target_dir);
		}
	}
	if (new_pwd == NULL)
		return (E_SYSTEM);
	free(d->pwd);
	d->pwd = new_pwd;
	if (set_oldpwd_var(d->vars_list, place) == E_SYSTEM
		|| set_pwd_var(d, place) == E_SYSTEM)
	{
		return (E_SYSTEM);
	}
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
