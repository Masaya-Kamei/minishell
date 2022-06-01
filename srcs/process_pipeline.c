/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 15:32:00 by mkamei            #+#    #+#             */
/*   Updated: 2022/06/01 13:00:37 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dup_stdinout(int next_fd[2])
{
	next_fd[0] = dup(STDIN_FILENO);
	if (next_fd[0] == -1)
		return (-1);
	next_fd[1] = dup(STDOUT_FILENO);
	if (next_fd[1] == -1)
		return (-1);
	return (0);
}

static int	dup2_stdinout(const int read_fd, const int write_fd)
{
	if (dup2(read_fd, STDIN_FILENO) == -1
		|| close(read_fd) == -1
		|| dup2(write_fd, STDOUT_FILENO) == -1
		|| close(write_fd) == -1)
	{
		return (-1);
	}
	return (0);
}

t_status	process_pipeline(t_data *d, t_token *tokens, int start, int end)
{
	int		i;
	int		pipe_fd[2];
	int		next_fd[2];

	i = start;
	if (dup_stdinout(next_fd) == -1)
		return (E_SYSTEM);
	while (i <= end)
	{
		if (tokens[i].type == PIPE)
		{
			if (pipe(pipe_fd) == -1
				|| dup2_stdinout(next_fd[0], pipe_fd[1]) == -1)
				return (E_SYSTEM);
			d->pipe_read_fd = pipe_fd[0];
			process_command(d, tokens, start, i - 1);
			start = i + 1;
			next_fd[0] = pipe_fd[0];
		}
		i++;
	}
	if (dup2_stdinout(next_fd[0], next_fd[1]) == -1)
		return (E_SYSTEM);
	d->pipe_read_fd = -1;
	return (process_command(d, tokens, start, end));
}
