/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 15:32:00 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/23 10:51:26 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_status	process_pipeline(
	t_token *tokens, int start, int end, t_list *vars_list[3])
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
				return (E_DUP_CLOSE);
			status = process_pipeline(tokens, start, i - 1, vars_list);
			if (status != SUCCESS)
				return (status);
			if (dup2(pipe_fd[0], 0) == -1 || dup2(backup_write_fd, 1) == -1
				|| close(pipe_fd[0]) == -1 || close(backup_write_fd) == -1)
				return (E_DUP_CLOSE);
			return (process_command(tokens, i + 1, end, vars_list));
			//return (debug_process_command(tokens, i + 1, end, vars_list));
		}
		i--;
	}
	return (process_command(tokens, start, end, vars_list));
	//return (debug_process_command(tokens, start, end, vars_list));
}

static t_bool	is_redirect_token_type(t_token_type type)
{
	if (type == GREATER || type == D_GREATER || type == LESS || type == D_LESS)
		return (1);
	else
		return (0);
}

static t_status	check_syntax_error(t_token *tokens, char err_word[10])
{
	int		i;

	i = -1;
	while (err_word[0] == '\0' && tokens[++i].type != '\0')
	{
		if (tokens[i].type == PIPE && (i == 0 || tokens[i - 1].type != WORD))
			ft_strlcpy(err_word, "`|'", 4);
		else if (is_redirect_token_type(tokens[i].type) == 1
			&& i != 0 && is_redirect_token_type(tokens[i - 1].type) == 1)
		{
			if (tokens[i].type == GREATER)
				ft_strlcpy(err_word, "`>'", 4);
			else if (tokens[i].type == D_GREATER)
				ft_strlcpy(err_word, "`>>'", 5);
			else if (tokens[i].type == LESS)
				ft_strlcpy(err_word, "`<'", 4);
			else if (tokens[i].type == D_LESS)
				ft_strlcpy(err_word, "`<<'", 5);
		}
	}
	if (tokens[i].type == '\0' && tokens[i - 1].type != WORD)
		ft_strlcpy(err_word, "`newline'", 10);
	if (err_word[0] != '\0')
		return (E_SYNTAX);
	return (SUCCESS);
}

t_status	start_process(
	t_token *tokens, int start, int end, t_list *vars_list[3])
{
	int				backup_read_fd;
	t_status		status;
	t_exit_status	exit_status;
	char			err_word[10];

	if (end == -1)
		return (SUCCESS);
	err_word[0] = '\0';
	status = check_syntax_error(tokens, err_word);
	if (status == E_SYNTAX)
	{
		exit_status = get_exit_status_with_errout(err_word, E_SYNTAX, P_SHELL);
		set_exit_status(vars_list[SPECIAL], exit_status);
		return (SUCCESS);
	}
	backup_read_fd = dup(0);
	if (backup_read_fd == -1)
		return (E_DUP_CLOSE);
	status = process_pipeline(tokens, start, end, vars_list);
	if (status != SUCCESS)
		return (status);
	if (dup2(backup_read_fd, 0) == -1 || close(backup_read_fd) == -1)
		return (E_DUP_CLOSE);
	return (SUCCESS);
}
