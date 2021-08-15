/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:24:35 by keguchi           #+#    #+#             */
/*   Updated: 2021/08/10 15:29:52 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	envp = create_envp(vars_list[ENV]);
	if (envp == NULL)
		exit(get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL));
	execve(cmd_path, command, envp);
	exit(get_exit_status_with_errout(cmd_path, E_SYSTEM, P_SHELL));
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
	status = add_pid_list(&d->pid_list, pid);
	if (status == E_SYSTEM)
		return (E_SYSTEM);
	return (SUCCESS);
}

static t_status	edit_status_with_restore_fd(
	t_data *d, t_list *save_fd, char *err_word, t_status status)
{
	t_list			*list;
	int				redirect_fd;
	int				backup_fd;

	if (status == E_OPEN || status == E_AMBIGUOUS)
		set_exit_status_with_errout(err_word, status, P_SHELL, d->vars_list);
	if (status == SUCCESS || status == E_OPEN || status == E_AMBIGUOUS)
	{
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
	}
	ft_lstclear(&save_fd, free);
	return (status);
}

t_status	process_command(t_data *d, t_token *tokens, int start, int end)
{
	t_status		status;
	t_list			*save_fd;
	char			*cmd_str;
	char			**command;
	const t_bool	is_pipe = !(isatty(0) == 1 && isatty(1) == 1);

	save_fd = NULL;
	cmd_str = NULL;
	command = NULL;
	status = SUCCESS;
	while (status == SUCCESS && start <= end)
	{
		if (tokens[start].type == WORD)
			status = strjoin_to_cmd_str(tokens, start, &cmd_str, d->vars_list);
		else
			status = process_redirect(tokens, start++, &save_fd, d->vars_list);
		start++;
	}
	if (status == SUCCESS)
		status = split_cmd_str(cmd_str, &command);
	if (status == SUCCESS)
		status = exec_command(d, command, is_pipe);
	free(cmd_str);
	free_double_pointer((void **)command);
	return (edit_status_with_restore_fd(d, save_fd, tokens[start].str, status));
}
