/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 16:54:39 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/10 15:58:08 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	is_redirect_token_type(t_token_type type)
{
	if (type == GREATER || type == D_GREATER || type == LESS || type == D_LESS)
		return (1);
	else
		return (0);
}

static t_status	check_syntax_error(t_token *tokens, char **err_word)
{
	int		i;

	i = -1;
	while (*err_word == NULL && tokens[++i].type != '\0')
	{
		if (tokens[i].type == PIPE && (i == 0 || tokens[i - 1].type != WORD))
			*err_word = "`|'";
		else if (is_redirect_token_type(tokens[i].type) == 1
			&& i != 0 && is_redirect_token_type(tokens[i - 1].type) == 1)
		{
			if (tokens[i].type == GREATER)
				*err_word = "`>'";
			else if (tokens[i].type == D_GREATER)
				*err_word = "`>>'";
			else if (tokens[i].type == LESS)
				*err_word = "`<'";
			else if (tokens[i].type == D_LESS)
				*err_word = "`<<'";
		}
	}
	if (tokens[i].type == '\0' && tokens[i - 1].type != WORD)
		*err_word = "`newline'";
	if (*err_word != NULL)
		return (E_SYNTAX);
	return (SUCCESS);
}

t_status	start_process(t_data *d, t_token *tokens, int start, int end)
{
	int			backup_read_fd;
	t_status	status;
	char		*err_word;

	if (end == -1)
		return (SUCCESS);
	err_word = NULL;
	if (check_syntax_error(tokens, &err_word) == E_SYNTAX)
	{
		set_exit_status_with_errout(err_word, E_SYNTAX, P_SHELL, d->vars_list);
		return (SUCCESS);
	}
	// if (heredocument() == E_SYSTEM)
	// 	return (E_SYSTEM);
	backup_read_fd = dup(0);
	if (backup_read_fd == -1)
		return (E_SYSTEM);
	status = process_pipeline(d, tokens, start, end);
	if (status != SUCCESS)
		return (status);
	if (dup2(backup_read_fd, 0) == -1 || close(backup_read_fd) == -1)
		return (E_SYSTEM);
	if (d->pid_list != NULL)
		status = wait_child_process(&d->pid_list, d->vars_list);
	return (status);
}
