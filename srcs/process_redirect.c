/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:25:38 by keguchi           #+#    #+#             */
/*   Updated: 2021/09/02 13:58:14 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_redirect_num(char *str)
{
	int			i;
	long int	nbr;

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

static void	errout_in_redirect(char *err_token, t_status status,
	char *expanded_str, t_list *vars_list[3])
{
	char	*redirect_symbol_ptr;

	if (status == E_OVER_INT)
		set_exit_status_with_errout(OVER_INT_EMSG, status, vars_list);
	else if (status == E_OVER_LIMIT)
	{
		redirect_symbol_ptr = ft_strchr(err_token, '>');
		if (redirect_symbol_ptr != NULL)
			*redirect_symbol_ptr = '\0';
		redirect_symbol_ptr = ft_strchr(err_token, '<');
		if (redirect_symbol_ptr != NULL)
			*redirect_symbol_ptr = '\0';
	}
	else if (status == E_OPEN)
		set_exit_status_with_errout(expanded_str, status, vars_list);
}

t_status	process_redirect(t_token *tokens, int i,
	t_list **fds_list, t_list *vars_list[3])
{
	t_status		status;
	char			*expanded_str;
	t_expand_flag	flag;

	status = 0;
	flag = 0;
	if (tokens[i + 1].type == WORD)
		flag = EXPAND_QUOTE | EXPAND_VAR;
	else if (tokens[i + 1].type == HEREDOC_D_QUOTE)
		flag = EXPAND_VAR;
	if (expand_word_token(
			tokens[i + 1].str, vars_list, flag, &expanded_str) == E_SYSTEM)
		return (E_SYSTEM);
	if (!expanded_str)
	{
		set_exit_status_with_errout(tokens[i + 1].str, status, vars_list);
		return (E_AMBIGUOUS);
	}
	status = redirect_to_file(tokens[i], fds_list, expanded_str);
	if (status != SUCCESS)
		errout_in_redirect(tokens[i].str, status, expanded_str, vars_list);
	free(expanded_str);
	return (status);
}
