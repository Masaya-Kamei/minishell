/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_commandline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 17:04:53 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/10 18:00:38 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_list(t_token *tokens, int start, int end)
{
	int		i;
	int		status;

	i = end;
	while (i >= 0)
	{
		if (tokens[i].type == SEMICOLON)
			break ;
		i--;
	}
	if (i < 0)
	{
		// return (process_pipeline(tokens, start, end));
		return (SUCCESS);
	}
	else
	{
		if (i == end)
			return (ERR_D_SEMICOLON);
		if (i - 1 < 0)
			return (ERR_SEMICOLON);
		status = process_list(tokens, start, i - 1);
		if (status != SUCCESS)
			return (status);
		// return (process_pipeline(tokens, i + 1, end));
		return (SUCCESS);
	}
}

int	process_commandline(t_token *tokens, int start, int end)
{
	if (tokens[end].type != SEMICOLON)
		return (process_list(tokens, start, end));
	else
	{
		if (end - 1 < 0)
			return (ERR_SEMICOLON);
		return (process_list(tokens, start, end - 1));
	}
}
