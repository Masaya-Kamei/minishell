/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:25:38 by keguchi           #+#    #+#             */
/*   Updated: 2021/08/25 04:10:49 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long int atoi_with_unsigned_int(char *str)
{
	int				i;
	unsigned long	nbr;

	i = 0;
	nbr = 0;
	while (str[i] >= '0' && str[i] <= '9')
		nbr = (nbr * 10) + str[i++] - '0';
	if (nbr > UINT_MAX)
		return (-1);
	return (nbr);
}

static t_status	set_redirect_and_save_fd(char *type, int fd, t_list **save_fd)
{
	long long int	*redirect_fd;
	t_list			*new;

	redirect_fd = malloc(sizeof(int) * 2);
	if (!redirect_fd)
		return (E_SYSTEM);
	new = NULL;
	if (ft_strncmp(type, "<", 2) == 0 || ft_strncmp(type, "<<", 3) == 0)
		redirect_fd[0] = STDIN_FILENO;
	else if (ft_strncmp(type, ">", 2) == 0 || ft_strncmp(type, ">>", 3) == 0)
		redirect_fd[0] = STDOUT_FILENO;
	else
		redirect_fd[0] = atoi_with_unsigned_int(type);
	redirect_fd[1] = dup(redirect_fd[0]);
	if (redirect_fd[1] == -1 && (redirect_fd[0] > 10496))
		return (E_OVER_LIMIT);
	if (redirect_fd[1] == -1 && (redirect_fd[0] > INT_MAX || redirect_fd[0] < 0))
		return (E_OVER_FD);
	if (redirect_fd[1] == -1)
		return (SUCCESS);
	new = ft_lstnew(redirect_fd);
	if (!new)
		return (E_SYSTEM);
	ft_lstadd_back(save_fd, new);
	printf("errno = %d, fd1 = %lld, fd2 = %lld\n", errno, redirect_fd[0], redirect_fd[1]);
	if (dup2(fd, redirect_fd[0]) == -1 || close(fd) == -1)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	pipe_to_stdout(char *readline)
{
	int	pipe_fd[2];

	pipe(pipe_fd);
	write(pipe_fd[1], readline, ft_strlen(readline));
	if (close(pipe_fd[1]) == -1 || dup2(pipe_fd[0], STDIN_FILENO) == -1
		|| close(pipe_fd[0]) == -1)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	open_and_redirect_file(t_token token,
		t_list **save_fd, char *expanded_str)
{
	int			fd;
	char		*str;
	char		*tmp;

	str = NULL;
	tmp = NULL;
	fd = 0;
	if (token.type == '>')
		fd = open(expanded_str, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (token.type == '<')
		fd = open(expanded_str, O_RDONLY);
	else if (token.type == 'G')
		fd = open(expanded_str, O_RDWR | O_CREAT | O_APPEND, 0644);
	else if (token.type == 'L')
	{
		if (pipe_to_stdout(expanded_str) != SUCCESS)
			return (E_SYSTEM);
	}
	if (fd < 0)
		return (E_OPEN);
	return (set_redirect_and_save_fd(token.str, fd, save_fd));
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
	//status = open_and_redirect_file(tokens, i + 1, save_fd);
	free(expanded_str);
	if (status == E_OPEN || status == E_AMBIGUOUS || status == E_OVER_FD || status == E_OVER_LIMIT)
	{
		printf("status = %d, [%s]\n", status, tokens[i + 1].str);
		if (status == E_OVER_FD)
			set_exit_status_with_errout("file descriptor out of range", status, vars_list);
		else if (status == E_OVER_LIMIT)
			set_exit_status_with_errout(ft_itoa(ft_atoi(tokens[i].str)), status, vars_list);
		else if (status == E_OPEN || status == E_AMBIGUOUS)
			set_exit_status_with_errout(tokens[i + 1].str, status, vars_list);
	}
	return (status);
}
