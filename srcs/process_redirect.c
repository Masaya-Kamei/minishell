/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:25:38 by keguchi           #+#    #+#             */
/*   Updated: 2021/08/18 17:39:36 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_status	set_redirect_and_save_fd(char *type, int fd, t_list **save_fd)
{
	int		*redirect_fd;
	t_list	*new;

	redirect_fd = malloc(sizeof(int) * 2);
	if (!redirect_fd)
		return (E_SYSTEM);
	new = NULL;
	if (ft_strncmp(type, "<", 2) == 0)
		redirect_fd[0] = STDIN_FILENO;
	else if (ft_strncmp(type, ">", 2) == 0 || ft_strncmp(type, ">>", 3) == 0)
		redirect_fd[0] = STDOUT_FILENO;
	else
		redirect_fd[0] = ft_atoi(type);
	redirect_fd[1] = dup(redirect_fd[0]);
	if (redirect_fd[1] == -1)
		return (E_SYSTEM);
	new = ft_lstnew(redirect_fd);
	if (!new)
		return (E_SYSTEM);
	ft_lstadd_back(save_fd, new);
	if (dup2(fd, redirect_fd[0]) == -1 || close(fd) == -1)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	wait_eof_loop(char *eof_word)
{
	char	*line;
	int		pipe_fd[2];

	pipe(pipe_fd);
	while (1)
	{
		line = NULL;
		line = readline("> ");
		if (line == NULL || g_received_signal == SIGINT)
			return (SUCCESS);
		if (ft_strncmp(eof_word, line, ft_strlen(line) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	if (close(pipe_fd[1]) == -1 || dup2(pipe_fd[0], STDIN_FILENO) == -1
		|| close(pipe_fd[0]) == -1)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	open_and_redirect_file(t_token *tokens,
	int file_index, t_list **save_fd)
{
	int			fd;
	char		*str;
	char		*tmp;
	t_status	status;

	str = NULL;
	tmp = NULL;
	fd = 0;
	if (tokens[file_index - 1].type == '>')
		fd = open(tokens[file_index].str, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (tokens[file_index - 1].type == '<')
		fd = open(tokens[file_index].str, O_RDONLY);
	else if (tokens[file_index - 1].type == 'G')
		fd = open(tokens[file_index].str, O_RDWR | O_CREAT | O_APPEND, 0644);
	else if (tokens[file_index - 1].type == 'L')
	{
		// rl_event_hook = &redisplay_prompt;
		status = wait_eof_loop(tokens[file_index].str);
		return (status);
	}
	if (fd < 0)
		return (E_OPEN);
	return (set_redirect_and_save_fd(tokens[file_index - 1].str, fd, save_fd));
}

t_status	process_redirect(t_token *tokens, int i,
	t_list **save_fd, t_list *vars_list[3])
{
	t_status	status;
	char		*original_token;

	status = 0;
	if (tokens[i].type == GREATER || tokens[i].type == D_GREATER
		|| tokens[i].type == LESS || tokens[i].type == D_LESS)
	{
		original_token = ft_strdup(tokens[i + 1].str);
		if (!original_token)
			return (E_SYSTEM);
		if (expand_word_token(&tokens[i + 1].str, vars_list) == E_SYSTEM)
			return (E_SYSTEM);
		if (!tokens[i + 1].str)
		{
			tokens[i + 1].str = original_token;
			return (E_AMBIGUOUS);
		}
		status = open_and_redirect_file(tokens, i + 1, save_fd);
		free(original_token);
	}
	if (status == E_SYSTEM && errno == EBADF)
		status = SUCCESS;
	return (status);
}
