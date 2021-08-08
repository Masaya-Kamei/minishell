/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:10:11 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/08 13:47:34 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_full_path(char *path, char *last_file)
{
	const int	path_len = ft_strlen(path);
	char		*full_path;
	char		*tmp;

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
	, char *path_value, t_path_check_func check_func, char **matched_path)
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

static t_bool	check_directory_exist(char *path)
{
	struct stat	stat_buf;

	if (stat(path, &stat_buf) == -1)
		return (0);
	if (S_ISDIR(stat_buf.st_mode))
		return (1);
	else
		return (0);
}

static t_status	change_dir(
	char **arg, char *var, char *matched_path, t_list *vars_list[3])
{
	char	*target_dir;

	if (var != NULL)
	{
		target_dir = get_var(vars_list, var);
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
		return (get_exit_status_with_errout(target_dir, E_CHDIR, P_CD));
	if (set_oldpwd_var(vars_list, P_CD) == E_SYSTEM
		|| set_pwd_var(vars_list, P_CD) == E_SYSTEM)
		return (get_exit_status_with_errout(NULL, E_SYSTEM, P_CD));
	if ((var && ft_strncmp(var, "OLDPWD", 7) == 0) || matched_path != NULL)
		ft_putendl_fd(get_var(vars_list, "PWD"), 1);
	return (0);
}

t_exit_status	mini_cd(char **argv, t_list *vars_list[3])
{
	char		*var;
	char		*cdpath_value;
	char		*matched_path;
	t_status	status;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP, P_CD));
	var = NULL;
	if (argv[1] == NULL)
		var = "HOME";
	else if (argv[1][0] == '-' && argv[1][1] == '\0')
		var = "OLDPWD";
	matched_path = NULL;
	cdpath_value = get_var(vars_list, "CDPATH");
	if (cdpath_value != NULL && var == NULL
		&& ft_strncmp(argv[1], ".", 2) != 0 && ft_strncmp(argv[1], "..", 3) != 0
		&& ft_strncmp(argv[1], "./", 2) != 0
		&& ft_strncmp(argv[1], "../", 3) != 0)
	{
		status = search_match_path_from_path_var(
				argv[1], cdpath_value, check_directory_exist, &matched_path);
		if (status == E_SYSTEM)
			return (get_exit_status_with_errout(NULL, E_SYSTEM, P_CD));
	}
	return (change_dir(&argv[1], var, matched_path, vars_list));
}

// gcc -Wall -Werror -Wextra mini_cd.c mini_unset.c mini_export.c ../var_env.c
//	../var_ope.c ../var_set_any.c ../var_utils.c ../free.c ../error.c
//	-I ../../include -I ../../libft/ ../../libft/libft.a

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*vars_list[3];
// 	int		exit_status;
// 	char	path[100];

// 	(void)argc;
// 	vars_list[ENV] = create_env_list(envp);
// 	vars_list[SHELL] = NULL;
// 	vars_list[SPECIAL] = lstnew_with_strdup("?=0  ");
// 	((char *)vars_list[SPECIAL]->content)[3] = '\0';
// 	argv[0] = "cd";
// 	exit_status = mini_cd(argv, vars_list);
// 	set_exit_status(vars_list[SPECIAL], exit_status);
// 	printf("%s\n", get_var(vars_list, "?"));
// 	if (exit_status == 0)
// 	{
// 		getcwd(path, 100);
// 		write(1, path, ft_strlen(path));
// 		write(1, "\n", 1);
// 		printf("PWD=%s\n", get_var(vars_list, "PWD"));
// 		printf("OLDPWD=%s\n", get_var(vars_list, "OLDPWD"));
// 	}
// 	argv[0] = "unset";
// 	argv[1] = "PWD";
// 	argv[2] = "OLDPWD";
// 	argv[3] = NULL;
// 	exit_status = mini_unset(argv, vars_list);
// 	printf("%d\n", exit_status);
// 	argv[0] = "cd";
// 	argv[1] = NULL;
// 	exit_status = mini_cd(argv, vars_list);
// 	printf("%d\n", exit_status);
// 	printf("OLDPWD=%s\n", get_var(vars_list, "OLDPWD"));
// 	printf("PWD=%s\n", get_var(vars_list, "PWD"));
// 	printf("%s\n", (char *)vars_list[SHELL]->content);
// 	printf("%s\n", (char *)vars_list[SHELL]->next->content);
// 	return (0);
// }
