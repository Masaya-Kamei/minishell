/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 16:49:34 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/13 16:08:40 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*search_list_from_env_list(
	t_list *env_list, char *env, int env_name_len)
{
	while (env_list != NULL)
	{
		if (ft_strncmp(env_list->content, env, env_name_len) == 0
			&& (((char *)env_list->content)[env_name_len] == '='
			|| ((char *)env_list->content)[env_name_len] == '\0'))
		{
			return (env_list);
		}
		env_list = env_list->next;
	}
	return (NULL);
}

char	*get_env_from_env_list(t_list *env_list, char *env_name)
{
	int		env_name_len;
	t_list	*target_list;

	env_name_len = ft_strlen(env_name);
	target_list = search_list_from_env_list(env_list, env_name, env_name_len);
	if (target_list == NULL)
		return (NULL);
	if (((char *)target_list->content)[env_name_len] == '=')
	{
		return (&((char *)target_list->content)[env_name_len + 1]);
	}
	return (NULL);
}

int	set_env_in_env_list(t_list *env_list, char *env)
{
	const char	*equal_pointer = ft_strchr(env, '=');
	int			env_name_len;
	t_list		*target_list;
	t_list		*last_list;

	if (equal_pointer == NULL)
		env_name_len = ft_strlen(env);
	else
		env_name_len = equal_pointer - env;
	target_list = search_list_from_env_list(env_list, env, env_name_len);
	if (target_list == NULL)
	{
		last_list = ft_lstlast(env_list);
		last_list->next = lstnew_with_strdup(env);
		if (last_list->next == NULL)
			return (ERR_MALLOC);
		return (SUCCESS);
	}
	if (equal_pointer == NULL)
		return (SUCCESS);
	free(target_list->content);
	target_list->content = ft_strdup(env);
	if (target_list->content == NULL)
		return (ERR_MALLOC);
	return (SUCCESS);
}

int	delete_oldpwd_env_value(t_list *env_list)
{
	t_list		*target_list;
	t_list		*last_list;

	target_list = search_list_from_env_list(env_list, "OLDPWD", 6);
	if (target_list == NULL)
	{
		last_list = ft_lstlast(env_list);
		last_list->next = lstnew_with_strdup("OLDPWD");
		if (last_list->next == NULL)
			return (ERR_MALLOC);
		return (SUCCESS);
	}
	((char *)target_list->content)[6] = '\0';
	return (SUCCESS);
}

int	countup_shlvl_env(t_list *env_list)
{
	char	*value;
	t_list	*target_list;
	t_list	*last_list;

	target_list = search_list_from_env_list(env_list, "SHLVL", 5);
	if (target_list == NULL)
	{
		last_list = ft_lstlast(env_list);
		last_list->next = lstnew_with_strdup("SHLVL=1");
		if (last_list->next == NULL)
			return (ERR_MALLOC);
		return (SUCCESS);
	}
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
