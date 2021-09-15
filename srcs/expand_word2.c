/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 15:53:24 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/09 12:22:27 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_status	strjoin_to_expand_list(char *substr, t_list **expand_list)
{
	char	*tmp;

	tmp = (char *)(*expand_list)->content;
	(*expand_list)->content = strjoin_with_null_support(tmp, substr);
	free(tmp);
	if ((*expand_list)->content == NULL)
		return (E_SYSTEM);
	return (0);
}

static t_status	advance_expand_list(t_list **expand_list)
{
	t_list	*new_list;

	if (((char *)(*expand_list)->content)[0] == '\0')
		return (SUCCESS);
	new_list = lstnew_with_strdup("");
	if (new_list == NULL)
		return (E_SYSTEM);
	(*expand_list)->next = new_list;
	*expand_list = (*expand_list)->next;
	return (SUCCESS);
}

static t_status	strjoin_to_expand_list_with_split(
	char *substr, t_list **expand_list)
{
	char		**strs;
	int			i;
	t_status	status;

	strs = ft_split(substr, ' ');
	if (strs == NULL)
		return (E_SYSTEM);
	i = -1;
	status = SUCCESS;
	while (status == SUCCESS && strs[++i] != NULL)
	{
		if ((i == 0 && substr[0] == ' ') || i >= 1)
			status = advance_expand_list(expand_list);
		if (status == SUCCESS)
			status = strjoin_to_expand_list(strs[i], expand_list);
	}
	free_double_pointer((void **)strs);
	if (status == SUCCESS && substr[ft_strlen(substr) - 1] == ' ')
		status = advance_expand_list(expand_list);
	return (status);
}

t_status	add_to_expand_list(char *substr,
		t_bool split_flag, t_list *vars_list[3], t_list **expand_list)
{
	char				*expanded_str;
	t_status			status;

	if (substr == NULL)
		return (E_SYSTEM);
	if (vars_list == NULL)
		expanded_str = substr;
	else
		expanded_str = get_var(vars_list, &substr[1]);
	if (split_flag && vars_list && expanded_str && ft_strchr(expanded_str, ' '))
		status = strjoin_to_expand_list_with_split(expanded_str, expand_list);
	else
		status = strjoin_to_expand_list(expanded_str, expand_list);
	free(substr);
	return (status);
}
