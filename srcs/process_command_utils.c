/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 13:09:34 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/07 11:41:34 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strjoin_with_null_support(char *s1, char *s2)
{
	if (s1 == NULL && s2 == NULL)
		return (ft_strdup(""));
	else if (s1 == NULL)
		return (ft_strdup(s2));
	else if (s2 == NULL)
		return (ft_strdup(s1));
	return (ft_strjoin(s1, s2));
}

t_status	strjoin_to_cmd_str(
	t_token *tokens, int word_index, char **cmd_str, t_list *vars_list[3])
{
	char		*tmp;

	if (expand_word_token(&tokens[word_index].str, vars_list) == E_SYSTEM)
		return (E_SYSTEM);
	if (tokens[word_index].str == NULL)
		return (SUCCESS);
	else if ((tokens[word_index].str)[0] == '\0')
	{
		free(tokens[word_index].str);
		tokens[word_index].str = ft_strdup("\21");
		if (tokens[word_index].str == NULL)
			return (E_SYSTEM);
	}
	tmp = *cmd_str;
	*cmd_str = strjoin_with_null_support(tmp, " ");
	free(tmp);
	if (*cmd_str == NULL)
		return (E_SYSTEM);
	tmp = *cmd_str;
	*cmd_str = strjoin_with_null_support(tmp, tokens[word_index].str);
	free(tmp);
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

t_status	restore_fd(t_list *save_fd)
{
	t_list		*list;
	int			redirect_fd;
	int			backup_fd;
	t_status	status;

	list = save_fd;
	status = SUCCESS;
	while (status == SUCCESS && list != NULL)
	{
		redirect_fd = ((int *)list->content)[0];
		backup_fd = ((int *)list->content)[1];
		if (dup2(backup_fd, redirect_fd) == -1 || close(backup_fd) == -1)
			status = E_SYSTEM;
		list = list->next;
	}
	return (status);
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
