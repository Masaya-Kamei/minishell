/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 16:54:39 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/11 19:07:51 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_status	check_syntax_error(t_token *tokens, char **err_word)
{
	int		i;

	i = -1;
	while (*err_word == NULL && tokens[++i].type != '\0')
	{
		if (tokens[i].type == PIPE && (i == 0 || tokens[i - 1].type != WORD))
			*err_word = "`|'";
		else if (is_redirect_token(tokens[i].type) == 1
			&& i != 0 && is_redirect_token(tokens[i - 1].type) == 1)
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

static t_status	wait_child_process(t_list **pid_list, t_list *vars_list[3])
{
	t_list		*current_list;
	int			status;
	t_bool		sigint_flag;

	sigint_flag = 0;
	current_list = *pid_list;
	while (current_list != NULL)
	{
		if (waitpid(*(pid_t *)current_list->content, &status, 0) == -1)
			return (E_SYSTEM);
		sigint_flag |= (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT);
		current_list = current_list->next;
	}
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = WTERMSIG(status) + 128;
	else
		status = get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL);
	set_exit_status(vars_list[SPECIAL], status);
	write(1, "\n", sigint_flag);
	write(1, "Quit: 3\n", (status == (SIGQUIT + 128)) * 8);
	ft_lstclear(pid_list, free);
	*pid_list = NULL;
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
