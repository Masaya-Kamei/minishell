/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:25:38 by keguchi           #+#    #+#             */
/*   Updated: 2021/09/09 14:32:38 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_redirect_num(char *str)
{
	int		i;
	long	nbr;

	if (ft_strncmp(str, "<", 2) == 0 || ft_strncmp(str, "<<", 3) == 0)
		return (STDIN_FILENO);
	else if (ft_strncmp(str, ">", 2) == 0 || ft_strncmp(str, ">>", 3) == 0)
		return (STDOUT_FILENO);
	else
	{
		i = 0;
		nbr = 0;
		while (str[i] >= '0' && str[i] <= '9' && nbr <= INT_MAX)
			nbr = (nbr * 10) + str[i++] - '0';
		if (nbr > INT_MAX)
			return (-1);
		return ((int)nbr);
	}
}

static t_status	set_redirect_and_save_fd(t_token redirect_token,
		t_list **fds_list, int target_fd)
{
	t_list	*new;
	int		*save_fd;

	save_fd = malloc(sizeof(int) * 2);
	if (!save_fd)
		return (E_SYSTEM);
	save_fd[0] = get_redirect_num(redirect_token.str);
	save_fd[1] = dup(save_fd[0]);
	if (save_fd[1] == -1 && save_fd[0] > 10496)
		return (free_and_return(save_fd, E_OVER_LIMIT));
	else if (save_fd[1] == -1 && save_fd[0] == -1)
		return (free_and_return(save_fd, E_OVER_INT));
	else if (save_fd[1] == -1)
		return (free_and_return(save_fd, SUCCESS));
	new = ft_lstnew(save_fd);
	if (!new)
		return (free_and_return(save_fd, E_SYSTEM));
	ft_lstadd_front(fds_list, new);
	if (dup2(target_fd, save_fd[0]) == -1 || close(target_fd) == -1)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	redirect_to_file(t_token redirect_token,
		t_list **fds_list, char *expanded_str)
{
	int		target_fd;
	int		pipe_fd[2];

	target_fd = 0;
	if (redirect_token.type == GREATER)
		target_fd = open(expanded_str, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (redirect_token.type == LESS)
		target_fd = open(expanded_str, O_RDONLY);
	else if (redirect_token.type == D_GREATER)
		target_fd = open(expanded_str, O_RDWR | O_CREAT | O_APPEND, 0644);
	else if (redirect_token.type == D_LESS)
	{
		if (pipe(pipe_fd) < 0)
			return (E_SYSTEM);
		write(pipe_fd[1], expanded_str, ft_strlen(expanded_str));
		target_fd = pipe_fd[0];
		if (close(pipe_fd[1]) == -1)
			return (E_SYSTEM);
	}
	if (target_fd < 0)
		return (E_OPEN);
	return (set_redirect_and_save_fd(redirect_token, fds_list, target_fd));
}

static void	errout_in_redirect(char *redirect_token_str,
	t_list *expand_list, t_status status, t_list *vars_list[3])
{
	if (status == E_OVER_INT)
		set_exit_status_with_errout(OVER_INT_EMSG, status, vars_list);
	else if (status == E_OVER_LIMIT)
		set_exit_status_with_errout(redirect_token_str, status, vars_list);
	else if (status == E_OPEN)
		set_exit_status_with_errout(expand_list->content, status, vars_list);
}

t_status	process_redirect(
	t_token *tokens, int i, t_list **fds_list, t_list *vars_list[3])
{
	t_status		status;
	t_expand_flag	flag;
	t_list			*expand_list;

	flag = EXPAND_QUOTE | EXPAND_VAR | EXPAND_SPLIT;
	if (tokens[i].type == D_LESS && tokens[i + 1].type == WORD_RAW)
		flag = EXPAND_VAR;
	else if (tokens[i].type == D_LESS && tokens[i + 1].type == WORD_QUOTE)
		flag = 0;
	status = expand_word_token(tokens[i + 1], vars_list, flag, &expand_list);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	else if (expand_list == NULL || ft_lstsize(expand_list) >= 2)
	{
		set_exit_status_with_errout(tokens[i + 1].str, E_AMBIGUOUS, vars_list);
		ft_lstclear(&expand_list, free);
		return (E_AMBIGUOUS);
	}
	status = redirect_to_file(tokens[i], fds_list, expand_list->content);
	if (status != SUCCESS)
		errout_in_redirect(tokens[i].str, expand_list, status, vars_list);
	ft_lstclear(&expand_list, free);
	return (status);
}
