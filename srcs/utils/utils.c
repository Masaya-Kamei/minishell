/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 10:45:21 by keguchi           #+#    #+#             */
/*   Updated: 2021/08/10 19:06:38 by mkamei           ###   ########.fr       */
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

char	*create_full_path(char *path, char *last_file)
{
	int		path_len;
	char	*full_path;
	char	*tmp;

	if (path == NULL)
		return (ft_strdup(last_file));
	else if (last_file == NULL)
		return (ft_strdup(path));
	path_len = ft_strlen(path);
	if (path[path_len - 1] == '/')
		return (ft_strjoin(path, last_file));
	full_path = ft_strjoin(path, "/");
	if (full_path == NULL)
		return (NULL);
	tmp = full_path;
	full_path = ft_strjoin(tmp, last_file);
	free(tmp);
	if (full_path == NULL)
		return (NULL);
	return (full_path);
}

t_status	search_match_path_from_path_var(char *last_file
	, char *path_value, t_file_check_func check_func, char **matched_path)
{
	char	**paths;
	char	*full_path;
	int		i;

	paths = ft_split(path_value, ':');
	if (paths == NULL)
		return (E_SYSTEM);
	i = -1;
	*matched_path = NULL;
	while (*matched_path == NULL && paths[++i] != NULL)
	{
		full_path = create_full_path(paths[i], last_file);
		if (full_path == NULL)
		{
			free_double_pointer((void **)paths);
			return (E_SYSTEM);
		}
		if (check_func(full_path) == 1)
			*matched_path = full_path;
		else
			free(full_path);
	}
	free_double_pointer((void **)paths);
	return (SUCCESS);
}
