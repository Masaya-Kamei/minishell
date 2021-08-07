/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:10:11 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/07 11:20:29 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_status	set_var_and_write_dir(t_list *vars_list[3], t_bool write_flag)
{
	t_status	status;

	status = set_oldpwd_var(vars_list, 0);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	status = set_pwd_var(vars_list, 0);
	if (status != SUCCESS)
		return (status);
	if (write_flag == 1)
		ft_putendl_fd(get_var(vars_list, "PWD"), 1);
	return (SUCCESS);
}

static t_status	change_dir(char *arg, char *var, t_list *vars_list[3])
{
	char		*target_dir;
	t_bool		write_flag;

	if (var != NULL)
	{
		target_dir = get_var(vars_list, var);
		if (target_dir == NULL)
			return (E_NOSET_VAR);
	}
	else
		target_dir = arg;
	if (chdir(target_dir) == -1)
		return (E_CHDIR);
	if (var != NULL && ft_strncmp(var, "OLDPWD", 7) == 0)
		write_flag = 1;
	else
		write_flag = 0;
	return (set_var_and_write_dir(vars_list, write_flag));
}

static t_status	create_full_path(char *cdpath, char *arg, char **target_dir)
{
	char	*tmp;

	*target_dir = ft_strjoin(cdpath, "/");
	if (*target_dir == NULL)
		return (E_SYSTEM);
	tmp = *target_dir;
	*target_dir = ft_strjoin(tmp, arg);
	free(tmp);
	if (*target_dir == NULL)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	change_dir_from_cdpath(
	char *arg, const char *cdpath_var, t_list *vars_list[3])
{
	char	**cdpaths;
	char	*target_dir;
	t_bool	changed_flag;
	int		i;

	cdpaths = ft_split(cdpath_var, ':');
	if (cdpaths == NULL)
		return (E_SYSTEM);
	i = -1;
	changed_flag = 0;
	while (changed_flag == 0 && cdpaths[++i] != NULL)
	{
		if (create_full_path(cdpaths[i], arg, &target_dir) == E_SYSTEM)
		{
			free_double_pointer((void **)cdpaths);
			return (E_SYSTEM);
		}
		if (chdir(target_dir) == 0)
			changed_flag = 1;
		free(target_dir);
	}
	free_double_pointer((void **)cdpaths);
	if (changed_flag == 1)
		return (set_var_and_write_dir(vars_list, 1));
	return (change_dir(arg, NULL, vars_list));
}

t_exit_status	mini_cd(char **argv, t_list *vars_list[3])
{
	char		*var;
	t_status	status;
	const char	*cdpath_var = get_var(vars_list, "CDPATH");

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP, P_CD));
	var = NULL;
	if (argv[1] == NULL)
		var = "HOME";
	else if (argv[1][0] == '-' && argv[1][1] == '\0')
		var = "OLDPWD";
	if (cdpath_var == NULL
		|| ft_strncmp(argv[1], ".", 2) == 0 || ft_strncmp(argv[1], "..", 3) == 0
		|| ft_strncmp(argv[1], "./", 2) == 0
		|| ft_strncmp(argv[1], "../", 3) == 0)
		status = change_dir(argv[1], var, vars_list);
	else
		status = change_dir_from_cdpath(argv[1], cdpath_var, vars_list);
	if (status == E_NOSET_VAR)
		return (get_exit_status_with_errout(var, E_NOSET_VAR, P_CD));
	else if (status == E_CHDIR)
		return (get_exit_status_with_errout(argv[1], E_CHDIR, P_CD));
	else if (status != SUCCESS)
		return (get_exit_status_with_errout(NULL, status, P_CD));
	return (0);
}

	// target_dir = get_var(vars_list, var);
	// if (target_dir == NULL)
	// 	return (get_exit_status_with_errout(err_word, status, P_CD));
	// if (chdir(target_dir) == -1)
	// 	return (get_exit_status_with_errout(err_word, status, P_CD));
	// if (set_oldpwd_var(vars_list, 0) == E_SYSTEM
	// 	|| set_pwd_var(vars_list, 0) == E_SYSTEM)
	// 	return (get_exit_status_with_errout(NULL, E_SYSTEM, P_CD));
	// if (write_flag == 1)
	// 	ft_putendl_fd(get_var(vars_list, "PWD"), 1);
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
