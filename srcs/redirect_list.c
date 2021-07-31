/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:25:38 by keguchi           #+#    #+#             */
/*   Updated: 2021/07/31 17:01:08 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_redirect_and_save_fd(char *str, int *redirect_fd, int *save_fd)
{
	int	backup_fd;

	if ((ft_strncmp(str, "<", 2) == 0))
		*redirect_fd = STDIN_FILENO;
	else if ((ft_strncmp(str, ">", 2) == 0))
		*redirect_fd = STDOUT_FILENO;
	else if ((ft_strncmp(str, "<<", 3) == 0))
		*redirect_fd = STDIN_FILENO;
	else if ((ft_strncmp(str, ">>", 3) == 0))
		*redirect_fd = STDOUT_FILENO;
	else
		*redirect_fd = ft_atoi(str);
	backup_fd = dup(*redirect_fd);
	if (backup_fd == -1)
		return (backup_fd);
	save_fd[0] = *redirect_fd;
	save_fd[1] = backup_fd;
	return (backup_fd);
}

static int	redisplay_wait_eof(void)
{
	if (g_received_signal == SIGINT)
	{
		g_received_signal = 0;
		printf("\033[%dC\033[K", 2 + rl_end);
		rl_redisplay();
		rl_replace_line("\3", 1);
		rl_done = 1;
	}
	else if (g_received_signal == SIGQUIT)
	{
		g_received_signal = 0;
		printf("\033[%dC\033[K", 2 + rl_end);
		rl_redisplay();
	}
	return (0);
}

static int	heredoc_loop(char *line, char *eof_word, int fd, int status)
{
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strncmp(eof_word, line, ft_strlen(line) + 1) == 0)
			break ;
		if (line[0] == '\3')
		{
			g_received_signal = 0;
			status = SIGINT;
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (status);
}

static int	wait_eof(char *eof_word)
{
	int		status;
	int		fd;
	char	*line;

	status = 0;
	line = NULL;
	fd = open(";;input;;", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (E_OPEN);
	rl_event_hook = &redisplay_wait_eof;
	status = heredoc_loop(line, eof_word, fd, status);
	free(line);
	close(fd);
	return (status);
}

static t_status	remove_input_file(void)
{
	int	ret;

	ret = 0;
	ret = unlink(";;input;;");
	if (ret == -1)
		ret = E_UNLINK;
	return (SUCCESS);
}

static t_status	redirect_d_less(t_token *tokens,
	int file_index, int *save_fd, t_list *vars_list[3])
{
	t_exit_status	exit_status;
	int				fd;
	int				redirect_fd;
	int				backup_fd;

	backup_fd = set_redirect_and_save_fd(tokens[file_index - 1].str,
			&redirect_fd, save_fd);
	if (backup_fd == -1)
		return (E_DUP_CLOSE);
	if (wait_eof(tokens[file_index].str) == SIGINT)
		return (remove_input_file());
	fd = open(";;input;;", O_RDONLY);
	if (fd < 0)
	{
		exit_status = get_exit_status_with_errout(";;input;;", E_OPEN, P_SHELL);
		set_exit_status(vars_list[SPECIAL], exit_status);
		return (E_OPEN);
	}
	if (dup2(fd, redirect_fd) == -1 || close(fd) == -1)
		return (E_DUP_CLOSE);
	return (remove_input_file());
}

static t_status	redirect_greater_and_less(t_token *tokens,
	int file_index, int *save_fd, t_list *vars_list[3])
{
	int				fd;
	int				redirect_fd;
	int				backup_fd;
	t_exit_status	exit_status;

	backup_fd = set_redirect_and_save_fd(tokens[file_index - 1].str,
			&redirect_fd, save_fd);
	if (backup_fd == -1)
		return (E_DUP_CLOSE);
	if (tokens[file_index].type == '>')
		fd = open(tokens[file_index].str, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (tokens[file_index].type == '<')
		fd = open(tokens[file_index].str, O_RDONLY);
	else if (tokens[file_index].type == 'G')
		fd = open(tokens[file_index].str, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		exit_status = get_exit_status_with_errout(tokens[file_index + 1].str,
				E_OPEN, P_SHELL);
		set_exit_status(vars_list[SPECIAL], exit_status);
		return (E_OPEN);
	}
	if (dup2(fd, redirect_fd) == -1 || close(fd) == -1)
		return (E_DUP_CLOSE);
	return (SUCCESS);
}

t_status	redirect_list(t_token *tokens, int start_index,
	int end_index, int **save_fd, t_list *vars_list[3])
{
	t_status	status;
	int			i;

	status = 0;
	i = 0;
	while (start_index <= end_index)
	{
		if (tokens[start_index].type == '<' || tokens[start_index].type == '>'
			|| tokens[start_index].type == 'G')
			status = redirect_greater_and_less(tokens, start_index + 1,
					save_fd[i++], vars_list);
		else if (tokens[start_index].type == 'L')
			redirect_d_less(tokens, start_index + 1, save_fd[i++]);
		if (status == E_DUP_CLOSE && errno == EBADF)
			status = SUCCESS;
		if (status != SUCCESS)
			return (status);
		start_index++;
	}
	return (status);
}
