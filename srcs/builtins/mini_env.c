/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:37 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/02 14:12:24 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit_status	mini_env(t_data *d, char **argv)
{
	t_list	*current_list;

	if (argv[1] != NULL)
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP_ARG, P_ENV));
	current_list = d->vars_list[ENV];
	while (current_list != NULL)
	{
		if (ft_strchr(current_list->content, '=') != NULL)
			ft_putendl_fd(current_list->content, 1);
		current_list = current_list->next;
	}
	return (0);
}
