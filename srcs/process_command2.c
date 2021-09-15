/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 13:09:34 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/15 12:11:50 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_export_cmd_index(t_token *tokens, int word_index)
{
	int		start;
	int		export_cmd_index;

	start = word_index;
	while (start >= 0 && tokens[start].type != PIPE)
		start--;
	export_cmd_index = start + 1;
	while (is_word_token(tokens[export_cmd_index]) == 0)
		export_cmd_index += 2;
	return (export_cmd_index);
}

t_status	add_to_cmd_args(
	t_token *tokens, int i, t_list **args_list, t_list *vars_list[3])
{
	t_expand_flag	flag;
	t_list			*expand_list;
	char			*equal_ptr;
	int				j;

	flag = EXPAND_VAR | EXPAND_QUOTE | EXPAND_SPLIT;
	if (*args_list && ft_strncmp((*args_list)->content, "export", 7) == 0)
	{
		equal_ptr = ft_strchr(tokens[i].str, '=');
		if (equal_ptr)
			*equal_ptr = '\0';
		j = get_export_cmd_index(tokens, i);
		if (!ft_strchr(tokens[i].str, '\'') && !ft_strchr(tokens[i].str, '\"')
			&& !ft_strchr(tokens[i].str, '$') && !ft_strchr(tokens[j].str, '\'')
			&& !ft_strchr(tokens[j].str, '\"')
			&& !ft_strchr(tokens[j].str, '$'))
			flag &= ~EXPAND_SPLIT;
		if (equal_ptr)
			*equal_ptr = '=';
	}
	if (expand_word_token(tokens[i], vars_list, flag, &expand_list) == E_SYSTEM)
		return (E_SYSTEM);
	ft_lstadd_back(args_list, expand_list);
	return (SUCCESS);
}

t_bool	check_regular_file_exist(char *path)
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
