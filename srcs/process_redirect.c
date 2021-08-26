/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:25:38 by keguchi           #+#    #+#             */
/*   Updated: 2021/08/26 17:38:30 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long long int	get_redirect_num(char *str)
{
	int				i;
	unsigned long	nbr;

	i = 0;
	nbr = 0;
	if (ft_strncmp(str, "<", 2) == 0 || ft_strncmp(str, "<<", 3) == 0)
		return (STDIN_FILENO);
	else if (ft_strncmp(str, ">", 2) == 0 || ft_strncmp(str, ">>", 3) == 0)
		return (STDOUT_FILENO);
	while (str[i] >= '0' && str[i] <= '9')
		nbr = (nbr * 10) + str[i++] - '0';
	if (nbr > UINT_MAX)
		return (-1);
	return (nbr);
}

static t_status	set_redirect_and_save_fd(t_list **save_fd,
	int *red_fd, int fd, char *heredoc)
{
	t_list	*new;
	int		pipe_fd[2];

	red_fd[1] = dup(red_fd[0]);
	if (red_fd[1] == -1 && (red_fd[0] > 10496))
		return (E_OVER_LIMIT);
	if (red_fd[1] == -1 && (red_fd[0] > INT_MAX || red_fd[0] < 0))
		return (E_OVER_FD);
	if (red_fd[1] == -1)
		return (SUCCESS);
	new = ft_lstnew(red_fd);
	if (!new)
		return (E_SYSTEM);
	ft_lstadd_back(save_fd, new);
	if (fd == -1)
	{
		pipe(pipe_fd);
		write(pipe_fd[1], heredoc, ft_strlen(heredoc));
		if (close(pipe_fd[1]) == -1 || dup2(pipe_fd[0], red_fd[0]) == -1
			|| close(pipe_fd[0]) == -1)
			return (E_SYSTEM);
	}
	else if (dup2(fd, red_fd[0]) == -1 || close(fd) == -1)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	open_and_redirect_file(t_token token,
		t_list **save_fd, char *expanded_str)
{
	int		fd;
	char	*str;
	char	*tmp;
	int		*red_fd;

	str = NULL;
	tmp = NULL;
	fd = 0;
	red_fd = malloc(sizeof(long long int) * 2);
	if (!red_fd)
		return (E_SYSTEM);
	red_fd[0] = get_redirect_num(token.str);
	if (token.type == '>')
		fd = open(expanded_str, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (token.type == '<')
		fd = open(expanded_str, O_RDONLY);
	else if (token.type == 'G')
		fd = open(expanded_str, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (E_OPEN);
	if (token.type == 'L')
		fd = -1;
	return (set_redirect_and_save_fd(save_fd, red_fd, fd, expanded_str));
}

t_status	process_redirect(t_token *tokens, int i,
	t_list **save_fd, t_list *vars_list[3])
{
	t_status	status;
	char		*expanded_str;

	status = 0;
	if (expand_word_token(tokens[i + 1].str, vars_list, 0, &expanded_str)
		 == E_SYSTEM)
		return (E_SYSTEM);
	if (!expanded_str)
		status = E_AMBIGUOUS;
	if (status == SUCCESS)
		status = open_and_redirect_file(tokens[i], save_fd, expanded_str);
	free(expanded_str);
	if (status == E_OVER_FD)
		set_exit_status_with_errout("file descriptor out of range",
			status, vars_list);
	else if (status == E_OVER_LIMIT)
		set_exit_status_with_errout(ft_itoa(ft_atoi(tokens[i].str)),
			status, vars_list);
	else if (status == E_AMBIGUOUS)
		set_exit_status_with_errout(tokens[i + 1].str, status, vars_list);
	else if (status == E_OPEN)
		set_exit_status_with_errout("", status, vars_list);
	return (status);
}
