/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 18:28:52 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/17 14:07:50 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_pointer(void **p)
{
	int		i;

	if (p == NULL)
		return ;
	i = 0;
	while (p[i] != NULL)
	{
		free(p[i]);
		i++;
	}
	free(p);
}

void	free_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].type != '\0')
	{
		free(tokens[i].str);
		i++;
	}
	free(tokens);
}

t_status	free_and_return(void *p, t_status status)
{
	free(p);
	return (status);
}

void	free_and_fill_null(char **p)
{
	free(*p);
	*p = NULL;
}

void	clear_shell_data(t_data *d)
{
	free(d->pwd);
	ft_lstclear(&d->pid_list, free);
	ft_lstclear(&d->vars_list[ENV], free);
	ft_lstclear(&d->vars_list[SHELL], free);
	ft_lstclear(&d->vars_list[SPECIAL], free);
}
