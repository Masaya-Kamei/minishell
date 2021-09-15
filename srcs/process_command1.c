/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:24:35 by keguchi           #+#    #+#             */
/*   Updated: 2021/09/15 12:12:33 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_status	add_to_cmd_args(t_token *tokens,
				int i, t_list **args_list, t_list *vars_list[3]);
t_bool		check_regular_file_exist(char *path);
t_status	search_command_path(
				char *cmd_name, t_list *vars_list[3], char **cmd_path);
t_status	add_to_pid_list(t_list **pid_list, pid_t pid);

static t_builtin_func	check_builtin_command(char *cmd_name)
{
	if (cmd_name == NULL)
		return (NULL);
	else if (ft_strncmp(cmd_name, "echo", 5) == 0)
		return (mini_echo);
	else if (ft_strncmp(cmd_name, "cd", 3) == 0)
		return (mini_cd);
	else if (ft_strncmp(cmd_name, "pwd", 4) == 0)
		return (mini_pwd);
	else if (ft_strncmp(cmd_name, "export", 7) == 0)
		return (mini_export);
	else if (ft_strncmp(cmd_name, "unset", 6) == 0)
		return (mini_unset);
	else if (ft_strncmp(cmd_name, "env", 4) == 0)
		return (mini_env);
	else if (ft_strncmp(cmd_name, "exit", 5) == 0)
		return (mini_exit);
	else
		return (NULL);
}

static void	exec_external_command(char **command, t_list *vars_list[3])
{
	t_status	status;
	char		*cmd_path;
	char		**envp;

	if (command[0] == NULL)
		exit(0);
	status = search_command_path(command[0], vars_list, &cmd_path);
	if (status == E_SYSTEM)
		exit(get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL));
	else if (status == E_NOCOMMAND)
		exit(get_exit_status_with_errout(command[0], E_NOCOMMAND, P_SHELL));
	status = convert_list_to_strs(vars_list[ENV], &envp);
	if (status == E_SYSTEM)
		exit(get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL));
	execve(cmd_path, command, envp);
	if (check_directory_exist(cmd_path) == 1)
		status = E_IS_DIR;
	else if (check_regular_file_exist(cmd_path) == 1)
		status = E_IS_FILE;
	else
		status = E_NO_PATHCOMMAND;
	exit(get_exit_status_with_errout(cmd_path, status, P_SHELL));
}

static t_status	exec_command(t_data *d, char **command, t_bool is_pipe)
{
	pid_t					pid;
	t_status				status;
	t_exit_status			exit_status;
	const t_builtin_func	builtin_func = check_builtin_command(command[0]);

	if (is_pipe == 0 && builtin_func != NULL)
	{
		exit_status = builtin_func(d, command);
		set_exit_status(d->vars_list[SPECIAL], exit_status);
		return (SUCCESS);
	}
	pid = fork();
	if (pid < 0)
		return (E_SYSTEM);
	else if (pid == 0)
	{
		if (builtin_func != NULL)
			exit(builtin_func(d, command));
		else
			exec_external_command(command, d->vars_list);
	}
	status = add_to_pid_list(&d->pid_list, pid);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	restore_fds(t_list *fds_list, t_status status)
{
	t_list			*list;
	int				redirect_fd;
	int				backup_fd;

	if (status == SUCCESS || status == E_OPEN || status == E_AMBIGUOUS
		|| status == E_OVER_INT || status == E_OVER_LIMIT)
	{
		list = fds_list;
		status = SUCCESS;
		while (status == SUCCESS && list != NULL)
		{
			redirect_fd = ((int *)list->content)[0];
			backup_fd = ((int *)list->content)[1];
			if (dup2(backup_fd, redirect_fd) == -1 || close(backup_fd) == -1)
				status = E_SYSTEM;
			list = list->next;
		}
	}
	ft_lstclear(&fds_list, free);
	return (status);
}

t_status	process_command(t_data *d, t_token *tokens, int start, int end)
{
	t_status		status;
	t_list			*fds_list;
	t_list			*args_list;
	char			**command;
	const t_bool	is_pipe = !(isatty(0) == 1 && isatty(1) == 1);

	fds_list = NULL;
	args_list = NULL;
	command = NULL;
	status = SUCCESS;
	start -= 1;
	while (status == SUCCESS && ++start <= end)
	{
		if (is_word_token(tokens[start]))
			status = add_to_cmd_args(tokens, start, &args_list, d->vars_list);
		else
			status = process_redirect(tokens, start++, &fds_list, d->vars_list);
	}
	if (status == SUCCESS)
		status = convert_list_to_strs(args_list, &command);
	if (status == SUCCESS)
		status = exec_command(d, command, is_pipe);
	ft_lstclear(&args_list, free);
	free_double_pointer((void **)command);
	return (restore_fds(fds_list, status));
}
