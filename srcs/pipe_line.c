/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 08:58:18 by keguchi           #+#    #+#             */
/*   Updated: 2021/06/24 18:37:10 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_used_pipe(t_token *tokens, int *start_index)
{
	if (tokens[*start_index].type == '|')
		(*start_index)--;
}

static void	search_pipe_index(t_token *tokens, int *start_index)
{
	while (*start_index > 0)
	{
		if (tokens[*start_index].type == '|')
		{
			(*start_index)++;
			break ;
		}
		(*start_index)--;
	}
}

static int	make_pipe(t_token *tokens, int *start_index)
{
	int		status;
	int		pipe_fd[2];
	pid_t	pid;
	int		ret;
	int		end_index;

	ret = 0;
	skip_used_pipe(tokens, start_index);
	end_index = *start_index;
	search_pipe_index(tokens, start_index);
	if (*start_index == 0)
		;//ret = exec_command(tokens, *start_index, end_index);
	else
	{
		pipe(pipe_fd);
		pid = fork();
		if (pid < 0)
			return (ERR_FORK);
		else if (pid == 0)
		{
			close(pipe_fd[READ_FD]);
			dup2(pipe_fd[WRITE_FD], STDOUT_FILENO);
			close(pipe_fd[WRITE_FD]);
			(*start_index)--;
			ret = make_pipe(tokens, start_index);
		}
		else
		{
			wait(&status);
			close(pipe_fd[WRITE_FD]);
			dup2(pipe_fd[READ_FD], STDIN_FILENO);
			close(pipe_fd[READ_FD]);
			//ret = exec_command(tokens, *start_index, end_index);
		}
	}
	return (ret);
}

int	pipe_line(t_token *tokens, int token_num)
{
	pid_t	pid;
	int		status;
	int		ret;
	int		start_index;

	ret = 0;
	start_index = token_num;
	pid = fork();
	if (pid < 0)
		return (ERR_FORK);
	else if (pid == 0)
		ret = make_pipe(tokens, &start_index);
	else
		wait(&status);
	return (ret);
}