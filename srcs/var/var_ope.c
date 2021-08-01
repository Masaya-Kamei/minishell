/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_ope.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 16:49:34 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/01 12:53:42 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var(t_list *vars_list[3], char *var_name)
{
	int		var_name_len;
	t_list	*target_list;
	int		i;

	i = 0;
	var_name_len = ft_strlen(var_name);
	while (i < 3)
	{
		target_list = get_target_list(vars_list[i], var_name, var_name_len);
		if (target_list != NULL
			&& ((char *)target_list->content)[var_name_len] == '=')
		{
			return (&((char *)target_list->content)[var_name_len + 1]);
		}
		i++;
	}
	return (NULL);
}

t_status	set_var(t_list *vars_list[3], char *var, t_vars_type var_type)
{
	const char	*equal_pointer = ft_strchr(var, '=');
	int			var_name_len;
	t_list		*target_list;

	if (equal_pointer == NULL)
		var_name_len = ft_strlen(var);
	else
		var_name_len = equal_pointer - var;
	if (var_type == SHELL)
	{
		target_list = get_target_list(vars_list[ENV], var, var_name_len);
		if (target_list == NULL)
			target_list = get_target_list(vars_list[SHELL], var, var_name_len);
	}
	else
		target_list = get_target_list(vars_list[var_type], var, var_name_len);
	if (target_list == NULL)
		return (add_new_var(&vars_list[var_type], var));
	if (equal_pointer == NULL)
		return (SUCCESS);
	free(target_list->content);
	target_list->content = ft_strdup(var);
	if (target_list->content == NULL)
		return (E_SYSTEM);
	return (SUCCESS);
}

void	delete_var(t_list *vars_list[3], char *var_name, t_vars_type var_type)
{
	const int	var_name_len = ft_strlen(var_name);
	t_list		*current_list;
	t_list		*prev_list;

	current_list = vars_list[var_type];
	prev_list = NULL;
	while (current_list != NULL)
	{
		if (ft_strncmp(current_list->content, var_name, var_name_len) == 0
			&& (((char *)current_list->content)[var_name_len] == '='
			|| ((char *)current_list->content)[var_name_len] == '\0'))
		{
			if (prev_list == NULL)
				vars_list[var_type] = current_list->next;
			else
				prev_list->next = current_list->next;
			free(current_list->content);
			free(current_list);
			return ;
		}
		prev_list = current_list;
		current_list = current_list->next;
	}
	if (var_type == SHELL)
		delete_var(vars_list, var_name, ENV);
}
