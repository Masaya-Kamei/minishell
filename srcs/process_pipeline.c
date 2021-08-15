/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 15:32:00 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/09 17:06:55 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_status	process_pipeline(t_data *d, t_token *tokens, int start, int end)
{
	int			i;
	int			pipe_fd[2];
	int			backup_write_fd;
	t_status	status;

	i = end;
	while (i >= start)
	{
		if (tokens[i].type == PIPE)
		{
			backup_write_fd = dup(1);
			if (backup_write_fd == -1 || pipe(pipe_fd) == -1
				|| dup2(pipe_fd[1], 1) == -1 || close(pipe_fd[1]) == -1)
				return (E_SYSTEM);
			status = process_pipeline(d, tokens, start, i - 1);
			if (status != SUCCESS)
				return (status);
			if (dup2(pipe_fd[0], 0) == -1 || dup2(backup_write_fd, 1) == -1
				|| close(pipe_fd[0]) == -1 || close(backup_write_fd) == -1)
				return (E_SYSTEM);
			return (process_command(d, tokens, i + 1, end));
		}
		i--;
	}
	return (process_command(d, tokens, start, end));
}
