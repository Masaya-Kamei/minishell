/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 13:09:34 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/01 17:38:37 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_status	strjoin_to_cmd_str(
	t_token *tokens, int word_index, char **cmd_str, t_list *vars_list[3])
{
	t_status	status;
	char		*tmp;

	status = expand_word_token(&tokens[word_index].str, vars_list);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	else if (tokens[word_index].str == NULL)
		return (SUCCESS);
	if (*cmd_str == NULL)
		*cmd_str = ft_strdup(tokens[word_index].str);
	else
	{
		tmp = *cmd_str;
		*cmd_str = ft_strjoin(tmp, " ");
		free(tmp);
		if (*cmd_str == NULL)
			return (E_SYSTEM);
		tmp = *cmd_str;
		*cmd_str = ft_strjoin(tmp, tokens[word_index].str);
		free(tmp);
	}
	if (*cmd_str == NULL)
		return (E_SYSTEM);
	return (SUCCESS);
}

char	**split_cmd_str(char *cmd_str)
{
	char	**command;

	if (cmd_str == NULL)
	{
		command = (char **)malloc(sizeof(char *) * 1);
		if (command == NULL)
			return (NULL);
		command[0] = NULL;
		return (command);
	}
	command = ft_split(cmd_str, ' ');
	return (command);
}

static int	count_redirect_token(t_token *tokens, int start, int end)
{
	int		i;
	int		redirect_count;

	redirect_count = 0;
	i = start;
	while (i <= end)
	{
		if (tokens[i].type == LESS || tokens[i].type == D_LESS
			|| tokens[i].type == GREATER || tokens[i].type == D_GREATER)
			redirect_count++;
		i++;
	}
	return (redirect_count);
}

t_status	malloc_to_save_fd(
	t_token *tokens, int start, int end, int ***save_fd)
{
	int		i;
	int		redirect_num;

	redirect_num = count_redirect_token(tokens, start, end);
	*save_fd = (int **)malloc(sizeof(int *) * (redirect_num + 1));
	if (*save_fd == NULL)
		return (E_SYSTEM);
	i = 0;
	while (i < redirect_num)
	{
		(*save_fd)[i] = (int *)malloc(sizeof(int) * 2);
		if ((*save_fd)[i] == NULL)
		{
			free_double_pointer((void **)*save_fd);
			*save_fd = NULL;
			return (E_SYSTEM);
		}
		i++;
	}
	(*save_fd)[i] = NULL;
	return (SUCCESS);
}

t_status	restore_fd(int **save_fd)
{
	int	i;

	i = 0;
	while (save_fd[i] != NULL)
	{
		if (dup2(save_fd[i][1], save_fd[i][0]) == -1
			|| close(save_fd[i][1]) == -1)
			return (E_SYSTEM);
		i++;
	}
	return (SUCCESS);
}
