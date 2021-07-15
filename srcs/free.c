/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 18:28:52 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 12:19:21 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_pointer(char **strs)
{
	int		i;

	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
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

int	free_and_return(void *p, int status)
{
	free(p);
	return (status);
}

void	free_and_fill_null(char **p)
{
	free(*p);
	*p = NULL;
}

void	clear_vars_list(t_list *vars_list[3])
{
	ft_lstclear(&vars_list[ENV], free);
	ft_lstclear(&vars_list[SHELL], free);
	ft_lstclear(&vars_list[SPECIAL], free);
}
