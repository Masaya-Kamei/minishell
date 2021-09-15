/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:10:11 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/14 19:51:05 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_full_path(char *path, char *last_file)
{
	int		path_len;

	if (path == NULL)
		return (ft_strdup(last_file));
	else if (last_file == NULL)
		return (ft_strdup(path));
	path_len = ft_strlen(path);
	if (path[path_len - 1] == '/')
		return (ft_strjoin(path, last_file));
	return (strjoin_three(path, "/", last_file));
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

t_bool	check_directory_exist(char *path)
{
	struct stat	stat_buf;

	if (stat(path, &stat_buf) == -1)
		return (0);
	if (S_ISDIR(stat_buf.st_mode))
		return (1);
	else
		return (0);
}

static t_status	change_dir(t_data *d, char **arg, char *var, char *matched_path)
{
	char	*target_dir;

	if (var != NULL)
	{
		target_dir = get_var(d->vars_list, var);
		if (target_dir == NULL)
			return (get_exit_status_with_errout(var, E_NOSET_VAR, P_CD));
	}
	else if (matched_path != NULL)
	{
		free(*arg);
		*arg = matched_path;
		target_dir = *arg;
	}
	else
		target_dir = *arg;
	if (chdir(target_dir) == -1)
		return (get_exit_status_with_errout(target_dir, E_SYSTEM, P_CD));
	if (set_pwd(d, P_CD, target_dir) == E_SYSTEM)
		return (get_exit_status_with_errout(NULL, E_SYSTEM, P_CD));
	if ((var && ft_strncmp(var, "OLDPWD", 7) == 0) || matched_path != NULL)
		ft_putendl_fd(d->pwd, 1);
	return (0);
}

t_exit_status	mini_cd(t_data *d, char **argv)
{
	char		*var;
	char		*cdpath_value;
	char		*matched_path;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP, P_CD));
	if (argv[1] != NULL && argv[1][0] == '\0')
		return (0);
	var = NULL;
	if (argv[1] == NULL)
		var = "HOME";
	else if (argv[1][0] == '-' && argv[1][1] == '\0')
		var = "OLDPWD";
	matched_path = NULL;
	cdpath_value = get_var(d->vars_list, "CDPATH");
	if (cdpath_value != NULL && var == NULL
		&& ft_strncmp(argv[1], ".", 2) != 0 && ft_strncmp(argv[1], "..", 3) != 0
		&& ft_strncmp(argv[1], "./", 2) != 0
		&& ft_strncmp(argv[1], "../", 3) != 0)
	{
		if (search_match_path_from_path_var(argv[1], cdpath_value
				, check_directory_exist, &matched_path) == E_SYSTEM)
			return (get_exit_status_with_errout(NULL, E_SYSTEM, P_CD));
	}
	return (change_dir(d, &argv[1], var, matched_path));
}
