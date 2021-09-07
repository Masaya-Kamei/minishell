/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 15:53:24 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/07 16:45:38 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_str_type	judge_str_type(
				char *word, int i, t_expand_flag flag, t_bool init_flag);

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
		{
			status = advance_expand_list(expand_list);
			if (status == E_SYSTEM)
				break ;
		}
		status = strjoin_to_expand_list(strs[i], expand_list);
	}
	free_double_pointer((void **)strs);
	if (status == SUCCESS && substr[ft_strlen(substr) - 1] == ' ')
		status = advance_expand_list(expand_list);
	return (status);
}

t_status	add_to_expand_list(
	char *substr_start, int len, t_list *vars_list[3], t_list **expand_list)
{
	char				*substr;
	t_status			status;
	const t_str_type	str_type_of_var = judge_str_type(substr_start, 0, 0, 0);
	const char			backup_char = substr_start[len];

	substr_start[len] = '\0';
	if (vars_list == NULL)
		substr = substr_start;
	else
		substr = get_var(vars_list, &substr_start[1]);
	if (vars_list && str_type_of_var == RAW && substr && ft_strchr(substr, ' '))
		status = strjoin_to_expand_list_with_split(substr, expand_list);
	else
		status = strjoin_to_expand_list(substr, expand_list);
	substr_start[len] = backup_char;
	return (status);
}
