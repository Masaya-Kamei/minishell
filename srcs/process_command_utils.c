/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 13:09:34 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/07 21:10:47 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static t_bool	check_regular_file_exist(char *path)
{
 	struct stat	stat_buf;

	if (stat(path, &stat_buf) == -1)
		return (0);
	if (S_ISREG(stat_buf.st_mode))
		return (1);
	else
		return (0);
}

t_status	search_command_path(
	char *cmd_name, t_list *vars_list[3], char **cmd_path)
{
	char		*path_value;
	t_status	status;
	char		*matched_path;

	path_value = get_var(vars_list, "PATH");
	if (path_value == NULL || ft_strchr(cmd_name, '/') != NULL)
	{
		*cmd_path = ft_strdup(cmd_name);
		if (*cmd_path == NULL)
			return (E_SYSTEM);
		return (SUCCESS);
	}
	matched_path = NULL;
	status = search_match_path_from_path_var(cmd_name, path_value
				, check_regular_file_exist, &matched_path);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	else if (matched_path == NULL)
		return (E_NOCOMMAND);
	*cmd_path = matched_path;
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
