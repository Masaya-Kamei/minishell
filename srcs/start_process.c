/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 16:54:39 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/09 14:34:56 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_status	check_syntax_error(t_token *tokens, char **err_word)
{
	int		i;

	i = -1;
	*err_word = NULL;
	while (*err_word == NULL && tokens[++i].type != '\0')
	{
		if (is_redirect_token(tokens[i]) && is_redirect_token(tokens[i + 1]))
		{
			if (tokens[i + 1].type == GREATER)
				*err_word = tokens[i + 1].str;
			if (tokens[i + 1].type == D_GREATER)
				*err_word = tokens[i + 1].str;
			else if (tokens[i + 1].type == LESS)
				*err_word = tokens[i + 1].str;
			else if (tokens[i + 1].type == D_LESS)
				*err_word = tokens[i + 1].str;
		}
		else if (is_word_token(tokens[i]) == 0 && tokens[i + 1].type == PIPE)
			*err_word = tokens[i + 1].str;
		else if (is_word_token(tokens[i]) == 0 && tokens[i + 1].type == '\0')
			*err_word = "newline";
	}
	if (*err_word != NULL)
		return (E_SYNTAX);
	return (SUCCESS);
}

static t_status	read_heredocument(
	char *eof, t_list *vars_list[3], char **heredoc)
{
	char	*line;
	char	*tmp;

	*heredoc = ft_strdup("");
	if (*heredoc == NULL)
		return (E_SYSTEM);
	line = readline("> ");
	while (line && ft_strncmp(eof, line, ft_strlen(eof) + 1) != 0)
	{
		if (line[0] == '\3')
		{
			free(line);
			set_exit_status(vars_list[SPECIAL], 1);
			return (E_SIG_INTERRUPT);
		}
		tmp = *heredoc;
		*heredoc = strjoin_three(tmp, line, "\n");
		free(tmp);
		free(line);
		if (*heredoc == NULL)
			return (E_SYSTEM);
		line = readline("> ");
	}
	free(line);
	return (SUCCESS);
}

static t_status	receive_heredocument(
	t_token *tokens, int start, int end, t_list *vars_list[3])
{
	int			i;
	t_list		*expand_list;
	t_status	status;

	i = start - 1;
	while (++i <= end)
	{
		if (tokens[i].type == D_LESS)
		{
			i++;
			if (expand_word_token(tokens[i]
					, vars_list, EXPAND_QUOTE, &expand_list) == E_SYSTEM)
				return (E_SYSTEM);
			free(tokens[i].str);
			status = read_heredocument(
					expand_list->content, vars_list, &tokens[i].str);
			ft_lstclear(&expand_list, free);
			if (status != SUCCESS)
				return (status);
		}
	}
	return (SUCCESS);
}

static t_status	wait_child_process(t_list *pid_list, t_list *vars_list[3])
{
	int			status;
	t_bool		sigint_flag;

	if (pid_list == NULL)
		return (SUCCESS);
	sigint_flag = 0;
	while (pid_list != NULL)
	{
		if (waitpid(*(pid_t *)pid_list->content, &status, 0) == -1)
			return (E_SYSTEM);
		sigint_flag |= (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT);
		pid_list = pid_list->next;
	}
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = WTERMSIG(status) + 128;
	else
		status = get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL);
	set_exit_status(vars_list[SPECIAL], status);
	if (sigint_flag == 1)
		write(1, "\n", sigint_flag);
	else if (status == SIGQUIT + 128)
		write(1, "Quit: 3\n", 8);
	return (SUCCESS);
}

t_status	start_process(t_data *d, t_token *tokens, int start, int end)
{
	int			backup_read_fd;
	t_status	status;
	char		*err_word;

	if (check_syntax_error(tokens, &err_word) == E_SYNTAX)
		return (set_exit_status_with_errout(err_word, E_SYNTAX, d->vars_list));
	status = receive_heredocument(tokens, start, end, d->vars_list);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	else if (status == E_SIG_INTERRUPT)
		return (SUCCESS);
	backup_read_fd = dup(0);
	if (backup_read_fd == -1
		|| signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		return (E_SYSTEM);
	status = process_pipeline(d, tokens, start, end);
	if (status != SUCCESS)
		return (status);
	if (dup2(backup_read_fd, 0) == -1 || close(backup_read_fd) == -1
		|| signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (E_SYSTEM);
	status = wait_child_process(d->pid_list, d->vars_list);
	ft_lstclear(&d->pid_list, free);
	d->pid_list = NULL;
	return (status);
}
