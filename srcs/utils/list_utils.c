/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 13:25:15 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/07 15:13:49 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_status	convert_strs_to_list(char **strs, t_list **list)
{
	int		i;
	t_list	*current_list;

	*list = lstnew_with_strdup(strs[0]);
	if (*list == NULL)
		return (E_SYSTEM);
	i = 1;
	current_list = *list;
	while (strs[i] != NULL)
	{
		current_list->next = lstnew_with_strdup(strs[i]);
		if (current_list->next == NULL)
		{
			ft_lstclear(list, free);
			*list = NULL;
			return (E_SYSTEM);
		}
		current_list = current_list->next;
		i++;
	}
	return (SUCCESS);
}

t_status	convert_list_to_strs(t_list *list, char ***strs)
{
	int		i;
	t_list	*current_list;

	*strs = (char **)malloc(sizeof(char *) * (ft_lstsize(list) + 1));
	if (*strs == NULL)
		return (E_SYSTEM);
	i = 0;
	current_list = list;
	while (current_list != NULL)
	{
		(*strs)[i] = ft_strdup(current_list->content);
		if ((*strs)[i] == NULL)
		{
			free_double_pointer((void **)*strs);
			*strs = NULL;
			return (E_SYSTEM);
		}
		i++;
		current_list = current_list->next;
	}
	(*strs)[i] = NULL;
	return (SUCCESS);
}

void	delete_last_list(t_list **list)
{
	t_list	*current_list;
	t_list	*new_last;

	if ((*list)->next == NULL)
	{
		free((*list)->content);
		free(*list);
		*list = NULL;
		return ;
	}
	current_list = *list;
	while (current_list != NULL)
	{
		if (current_list->next->next == NULL)
		{
			new_last = current_list;
			free(new_last->next->content);
			free(new_last->next);
			new_last->next = NULL;
			return ;
		}
		current_list = current_list->next;
	}
}
