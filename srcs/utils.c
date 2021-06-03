/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 18:28:52 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/03 18:08:00 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_escape(char *str, int i)
{
	int	escape_flag;

	if (i == 0)
		return (0);
	i--;
	escape_flag = 0;
	while (str[i] == '\\')
	{
		escape_flag = (escape_flag + 1) % 2;
		if (i == 0)
			break ;
		i--;
	}
	return (escape_flag);
}

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
	while (tokens[i].str != NULL)
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
