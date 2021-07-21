/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 12:19:39 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/20 11:34:37 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*get_target_list(t_list *any_list, char *var, int var_name_len)
{
	t_list	*current_list;

	current_list = any_list;
	while (current_list != NULL)
	{
		if (ft_strncmp(current_list->content, var, var_name_len) == 0
			&& (((char *)current_list->content)[var_name_len] == '='
			|| ((char *)current_list->content)[var_name_len] == '\0'))
		{
			return (current_list);
		}
		current_list = current_list->next;
	}
	return (NULL);
}

t_status	add_new_var(t_list **any_list, char *var)
{
	t_list	*new_list;
	t_list	*last_list;

	new_list = lstnew_with_strdup(var);
	if (new_list == NULL)
		return (E_MALLOC);
	last_list = ft_lstlast(*any_list);
	if (last_list == NULL)
		*any_list = new_list;
	else
		last_list->next = new_list;
	return (SUCCESS);
}

t_list	*lstnew_with_strdup(char *str)
{
	char	*content;
	t_list	*new_list;

	content = ft_strdup(str);
	if (content == NULL)
		return (NULL);
	new_list = ft_lstnew(content);
	if (new_list == NULL)
	{
		free(content);
		return (NULL);
	}
	return (new_list);
}