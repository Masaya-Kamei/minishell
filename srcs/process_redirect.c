/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:25:38 by keguchi           #+#    #+#             */
/*   Updated: 2021/08/03 17:00:02 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	heredoc_loop(char *line, char *eof_word, int fd, int status)
// {
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (line == NULL)
// 			break ;
// 		if (ft_strncmp(eof_word, line, ft_strlen(line) + 1) == 0)
// 			break ;
// 		if (line[0] == '\3')
// 		{
// 			g_received_signal = 0;
// 			status = SIGINT;
// 			break ;
// 		}
// 		write(fd, line, ft_strlen(line));
// 		write(fd, "\n", 1);
// 		free(line);
// 	}
// 	return (status);
// }

// static int	wait_eof(char *eof_word)
// {
// 	int		status;
// 	int		fd;
// 	char	*line;

// 	status = 0;
// 	line = NULL;
// 	fd = open(";;input;;", O_RDWR | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 		return (E_OPEN);
// 	rl_event_hook = &redisplay_prompt;
// 	status = heredoc_loop(line, eof_word, fd, status);
// 	free(line);
// 	close(fd);
// 	return (status);
// }

// static t_status	remove_input_file(void)
// {
// 	int	ret;

// 	ret = 0;
// 	ret = unlink(";;input;;");
// 	if (ret == -1)
// 		ret = E_SYSTEM;
// 	return (SUCCESS);
// }

static t_status	set_redirect_and_save_fd(char *type, int fd, t_list **save_fd)
{
	int		*redirect_fd;
	t_list	*new;

	redirect_fd = malloc(sizeof(int) * 2);
	if (!redirect_fd)
		return (E_SYSTEM);
	new = NULL;
	if (ft_strncmp(type, "<<", 3) == 0)
		return (SUCCESS);
	if (ft_strncmp(type, "<", 2) == 0)// || ft_strncmp(type, "<<", 3) == 0)
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

static t_status	wait_eof_loop(char *eof_word, char *str, char *tmp)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL || g_received_signal == SIGINT)
			return (SUCCESS);
		if (ft_strncmp(eof_word, line, ft_strlen(line) + 1) == 0)
			break ;
		tmp = str;
		str = strjoin_with_null_support(tmp, line);
		line = NULL;
		free(tmp);
		if (str == NULL)
			return (E_SYSTEM);
		tmp = str;
		str = strjoin_with_null_support(tmp, "\n");
		free(tmp);
		if (str == NULL)
			return (E_SYSTEM);
	}
	write(STDIN_FILENO, str, ft_strlen(str));
	free(line);
	free(str);
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
	else
	{
		rl_event_hook = &redisplay_prompt;
		status = wait_eof_loop(tokens[file_index].str, str, tmp);
		if (status != SUCCESS)
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
