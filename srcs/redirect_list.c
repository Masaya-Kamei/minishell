/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:25:38 by keguchi           #+#    #+#             */
/*   Updated: 2021/07/30 17:20:42 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_redirect_fd(char *str)
{
	if ((ft_strncmp(str, "<", 2) == 0))
		return (STDIN_FILENO);
	else if ((ft_strncmp(str, ">", 2) == 0))
		return (STDOUT_FILENO);
	else if ((ft_strncmp(str, "<<", 3) == 0))
		return (STDIN_FILENO);
	else if ((ft_strncmp(str, ">>", 3) == 0))
		return (STDOUT_FILENO);
	else
		return (ft_atoi(str));
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
	int redirect_index, int *save_fd)
{
	t_status	status;
	int			fd;
	int			redirect_fd;
	int			backup_fd;

	redirect_fd = set_redirect_fd(tokens[redirect_index].str);
	backup_fd = dup(redirect_fd);
	if (backup_fd == -1 && errno == EBADF)
		return (SUCCESS);
	else if (backup_fd == -1)
		return (E_DUP_CLOSE);
	save_fd[0] = redirect_fd;
	save_fd[1] = backup_fd;
	status = wait_eof(tokens[redirect_index + 1].str);
	if (status == SIGINT)
		return (remove_input_file());
	fd = open(";;input;;", O_RDONLY);
	if (fd < 0)
		return (E_OPEN);
	if (dup2(fd, redirect_fd) == -1 || close(fd) == -1)
		return (E_DUP_CLOSE);
	return (remove_input_file());
}

static t_status	redirect_greater_and_less(t_token *tokens,
	int redirect_index, int *save_fd, char type)
{
	int			fd;
	int			redirect_fd;
	int			backup_fd;

	redirect_fd = set_redirect_fd(tokens[redirect_index].str);
	backup_fd = dup(redirect_fd);
	if (backup_fd == -1 && errno == EBADF)
		return (SUCCESS);
	else if (backup_fd == -1)
		return (E_DUP_CLOSE);
	save_fd[0] = redirect_fd;
	save_fd[1] = backup_fd;
	if (type == '>')
		fd = open(tokens[redirect_index + 1].str, O_RDWR
				| O_CREAT | O_TRUNC, 0644);
	else if (type == '<')
		fd = open(tokens[redirect_index + 1].str, O_RDONLY);
	else if (type == 'G')
		fd = open(tokens[redirect_index + 1].str,
				O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (E_OPEN);
	if (dup2(fd, redirect_fd) == -1 || close(fd) == -1)
		return (E_DUP_CLOSE);
	return (SUCCESS);
}

t_status	redirect_list(t_token *tokens, int start_index,
	int end_index, int **save_fd)
{
	t_status	status;
	int			i;

	status = 0;
	i = 0;
	while (start_index <= end_index)
	{
		if (tokens[start_index].type == '<' || tokens[start_index].type == '>'
			|| tokens[start_index].type == 'G')
			status = redirect_greater_and_less(tokens, start_index,
					save_fd[i++], tokens[start_index].type);
		else if (tokens[start_index].type == 'L')
			redirect_d_less(tokens, start_index, save_fd[i++]);
		if (status != SUCCESS)
			return (status);
		start_index++;
	}
	return (status);
}
