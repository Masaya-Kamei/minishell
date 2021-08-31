/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:25:38 by keguchi           #+#    #+#             */
/*   Updated: 2021/08/31 11:12:43 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long long int	get_redirect_num(char *str)
{
	int				i;
	long long int	nbr;

	i = 0;
	nbr = 0;
	if (ft_strncmp(str, "<", 2) == 0 || ft_strncmp(str, "<<", 3) == 0)
		return (STDIN_FILENO);
	else if (ft_strncmp(str, ">", 2) == 0 || ft_strncmp(str, ">>", 3) == 0)
		return (STDOUT_FILENO);
	while (str[i] >= '0' && str[i] <= '9' && nbr <= INT_MAX)
		nbr = (nbr * 10) + str[i++] - '0';
	if (nbr > INT_MAX)
		return (-1);
	return (nbr);
}

static t_status	set_redirect_and_save_fd(t_list **save_fd,
	t_token token, int fd, int pipe_fd)
{
	t_list			*new;
	long long int	*red_fd;

	fd = pipe_fd;
	red_fd = malloc(sizeof(long long int) * 2);
	if (!red_fd)
		return (E_SYSTEM);
	red_fd[0] = get_redirect_num(token.str);
	red_fd[1] = dup(red_fd[0]);
	if (red_fd[1] == -1 && red_fd[0] > 10496)
		return (free_and_return(red_fd, E_OVER_LIMIT));
	if (red_fd[1] == -1 && red_fd[0] < 0)
		return (free_and_return(red_fd, E_OVER_FD));
	if (red_fd[1] == -1)
		return (free_and_return(red_fd, SUCCESS));
	new = ft_lstnew(red_fd);
	if (!new)
		return (free_and_return(red_fd, E_SYSTEM));
	ft_lstadd_front(save_fd, new);
	if (dup2(fd, red_fd[0]) == -1 || close(fd) == -1)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	open_and_redirect_file(t_token token,
		t_list **save_fd, char *expanded_str)
{
	int		fd;
	char	*str;
	char	*tmp;
	int		pipe_fd[2];

	str = NULL;
	tmp = NULL;
	fd = 0;
	if (token.type == '>')
		fd = open(expanded_str, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (token.type == '<')
		fd = open(expanded_str, O_RDONLY);
	else if (token.type == 'G')
		fd = open(expanded_str, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (E_OPEN);
	if (token.type == 'L')
	{
		if (pipe(pipe_fd) < 0)
			return (E_SYSTEM);
		write(pipe_fd[1], expanded_str, ft_strlen(expanded_str));
		if (close(pipe_fd[1]) == -1)
			return (E_SYSTEM);
	}
	return (set_redirect_and_save_fd(save_fd, token, fd, pipe[0]));
}

static void	redirect_replaced_null(char *str)
{
	char	*ret;

	ret = ft_strchr(str, '>');
	if (ret != NULL)
		*ret = '\0';
	ret = ft_strchr(str, '<');
	if (ret != NULL)
		*ret = '\0';
}

t_status	process_redirect(t_token *tokens, int i,
	t_list **save_fd, t_list *vars_list[3])
{
	t_status	status;
	char		*expanded_str;

	status = 0;
	if (expand_word_token(tokens[++i].str, vars_list, 0, &expanded_str)
		 == E_SYSTEM)
		return (E_SYSTEM);
	if (!expanded_str)
		status = E_AMBIGUOUS;
	if (status == SUCCESS)
		status = open_and_redirect_file(tokens[i - 1], save_fd, expanded_str);
	if (status == E_OVER_FD)
		set_exit_status_with_errout("file descriptor out of range",
			status, vars_list);
	if (status == E_OVER_LIMIT)
		redirect_replaced_null(tokens[--i].str);
	if (status == E_AMBIGUOUS || status == E_OVER_LIMIT)
		set_exit_status_with_errout(tokens[i].str, status, vars_list);
	if (status == E_OPEN)
		set_exit_status_with_errout(expanded_str, status, vars_list);
	free(expanded_str);
	return (status);
}
