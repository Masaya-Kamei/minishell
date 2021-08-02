/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 13:09:34 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/02 17:09:48 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_status	expand_word_with_replace(char **word, t_list *vars_list[3])
{
	t_status	status;

	status = expand_word_token(word, vars_list);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	if ((*word)[0] == '\0')
	{
		free(*word);
		*word = ft_strdup("\21");
		if (*word == NULL)
			return (E_SYSTEM);
	}
	return (SUCCESS);
}

t_status	strjoin_to_cmd_str(
	t_token *tokens, int word_index, char **cmd_str, t_list *vars_list[3])
{
	t_status	status;
	char		*tmp;

	status = expand_word_with_replace(&tokens[word_index].str, vars_list);
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

t_status	split_cmd_str(char *cmd_str, char ***command)
{
	int		i;

	if (cmd_str == NULL)
	{
		*command = (char **)malloc(sizeof(char *) * 1);
		if (*command == NULL)
			return (E_SYSTEM);
		(*command)[0] = NULL;
		return (SUCCESS);
	}
	*command = ft_split(cmd_str, ' ');
	if (*command == NULL)
		return (E_SYSTEM);
	i = 0;
	while ((*command)[i] != NULL)
	{
		if ((*command)[i][0] == '\21')
			(*command)[i][0] = '\0';
		i++;
	}
	return (SUCCESS);
}

t_exit_status	get_exit_status_when_signal(int signum)
{
	t_exit_status	exit_status;

	if (signum == SIGINT)
	{
		printf("\n");
		exit_status = 130;
	}
	else if (signum == SIGQUIT)
	{
		printf("Quit: 3\n");
		exit_status = 131;
	}
	else
		exit_status = 1;
	g_received_signal = 0;
	return (exit_status);
}
