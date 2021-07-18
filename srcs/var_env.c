/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 12:40:46 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 12:41:40 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*create_env_list(char **envp)
{
	int		i;
	t_list	*env_list;
	t_list	*current_list;

	env_list = lstnew_with_strdup(envp[0]);
	if (env_list == NULL)
		return (NULL);
	i = 1;
	current_list = env_list;
	while (envp[i] != NULL)
	{
		current_list->next = lstnew_with_strdup(envp[i]);
		if (current_list->next == NULL)
		{
			ft_lstclear(&env_list, free);
			return (NULL);
		}
		current_list = current_list->next;
		i++;
	}
	return (env_list);
}

char	**create_envp(t_list *env_list)
{
	int		i;
	int		env_num;
	char	**envp;

	env_num = ft_lstsize(env_list);
	envp = (char **)malloc(sizeof(char *) * (env_num + 1));
	if (envp == NULL)
		return (NULL);
	i = 0;
	while (env_list != NULL)
	{
		envp[i] = ft_strdup(env_list->content);
		if (envp[i] == NULL)
		{
			free_double_pointer(envp);
			return (NULL);
		}
		env_list = env_list->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
