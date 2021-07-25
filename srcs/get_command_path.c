/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 10:45:21 by keguchi           #+#    #+#             */
/*   Updated: 2021/07/23 08:01:30 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_cmd_path(char *cmd_name, char *dir_path)
{
	char	*result;
	char	*tmp;

	result = ft_strjoin(dir_path, "/");
	if (result == NULL)
		return (NULL);
	tmp = result;
	result = ft_strjoin(result, cmd_name);
	if (result == NULL)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (result);
}

static char	*read_dir(DIR *dir, char *cmd)
{
	struct dirent	*dp;

	dp = readdir(dir);
	while (dp != NULL)
	{
		if ((ft_strncmp(dp->d_name, cmd, ft_strlen(cmd) + 1) == 0))
			return (dp->d_name);
		dp = readdir(dir);
	}
	return (ft_strdup(""));
}

static char	*open_dir(char **paths, char *cmd)
{
	DIR		*dir;
	int		i;
	char	*result;

	i = 0;
	while (paths[i++] != NULL)
	{
		dir = opendir(paths[i]);
		if (dir == NULL)
			continue ;
		result = read_dir(dir, cmd);
		if ((ft_strncmp(result, "", 1)) != 0)
		{
			result = make_cmd_path(result, paths[i]);
			return (result);
		}
		closedir(dir);
		free(result);
	}
	return (ft_strdup(""));
}

char	*get_command_path(char *cmd)
{
	char	*env_path;
	char	**paths;
	char	*result;

	env_path = getenv("PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	result = open_dir(paths, cmd);
	free_double_pointer((void **)paths);
	return (result);
}
