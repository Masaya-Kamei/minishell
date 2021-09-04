/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 13:09:34 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/03 20:19:23 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_status	strjoin_to_cmd_str(
	t_token *tokens, int word_index, char **cmd_str, t_list *vars_list[3])
{
	char	*tmp;
	char	*expanded_str;

	if (expand_word_token(tokens[word_index], vars_list,
			EXPAND_VAR | EXPAND_QUOTE, &expanded_str) == E_SYSTEM)
		return (E_SYSTEM);
	if (expanded_str == NULL)
		return (SUCCESS);
	tmp = *cmd_str;
	*cmd_str = strjoin_with_null_support(tmp, " ");
	free(tmp);
	if (*cmd_str == NULL)
		return (E_SYSTEM);
	tmp = *cmd_str;
	if (expanded_str[0] == '\0')
		*cmd_str = strjoin_with_null_support(tmp, "\21");
	else
		*cmd_str = strjoin_with_null_support(tmp, expanded_str);
	free(tmp);
	free(expanded_str);
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
	status = search_match_path_from_path_var(cmd_name,
			path_value, check_regular_file_exist, &matched_path);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	else if (matched_path == NULL)
		return (E_NOCOMMAND);
	*cmd_path = matched_path;
	return (SUCCESS);
}

t_status	add_to_pid_list(t_list **pid_list, pid_t pid)
{
	pid_t	*pid_copy;
	t_list	*new_list;

	pid_copy = (pid_t *)malloc(sizeof(pid_t));
	if (pid_copy == NULL)
		return (E_SYSTEM);
	ft_memcpy(pid_copy, &pid, sizeof(pid_t));
	new_list = ft_lstnew(pid_copy);
	if (new_list == NULL)
	{
		free(pid_copy);
		return (E_SYSTEM);
	}
	ft_lstadd_back(pid_list, new_list);
	return (SUCCESS);
}
