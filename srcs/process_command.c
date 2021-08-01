/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:24:35 by keguchi           #+#    #+#             */
/*   Updated: 2021/08/01 17:40:22 by mkamei           ###   ########.fr       */
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
	char		*cmd_path;
	char		**envp;

	cmd_path = get_command_path(command[0]);
	if (cmd_path == NULL)
		exit(get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL));
	envp = create_envp(vars_list[ENV]);
	if (envp == NULL)
		exit(get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL));
	if (ft_strncmp(cmd_path, "", 1) == 0)
		exit(get_exit_status_with_errout(command[0], E_NOCOMMAND, P_SHELL));
	execve(cmd_path, command, envp);
	free(cmd_path);
	free_double_pointer((void **)envp);
	free_double_pointer((void **)command);
	exit(get_exit_status_with_errout(command[0], E_SYSTEM, P_SHELL));
}

static t_status	exec_command(char **command, int is_pipe, t_list *vars_list[3])
{
	pid_t					pid;
	t_exit_status			exit_status;
	const t_builtin_func	builtin_func = check_builtin_command(command[0]);

	if (command[0] == NULL)
		exit_status = 0;
	else if (is_pipe && builtin_func != NULL)
		exit_status = builtin_func(command, vars_list);
	else
	{
		pid = fork();
		if (pid < 0)
			return (E_SYSTEM);
		else if (pid == 0 && builtin_func != NULL)
			exit(builtin_func(command, vars_list));
		else if (pid == 0)
			exec_external_command(command, vars_list);
		wait(&exit_status);
		if ((WIFEXITED(exit_status)))
			exit_status = WEXITSTATUS(exit_status);
		else
			exit_status = get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL);
	}
	set_exit_status(vars_list[SPECIAL], exit_status);
	return (SUCCESS);
}

static t_status	clean_and_return(
	char *cmd_str, char **command, int **save_fd, t_status status)
{
	free(cmd_str);
	free_double_pointer((void **)command);
	free_double_pointer((void **)save_fd);
	if (status == E_OPEN || status == E_AMBIGUOUS)
		status = SUCCESS;
	return (status);
}

t_status	process_command(
	t_token *tokens, int start, int end, t_list *vars_list[3])
{
	t_status		status;
	int				**save_fd;
	char			*cmd_str;
	char			**command;
	const t_bool	is_pipe = !(isatty(0) == 1 && isatty(1) == 1);

	cmd_str = NULL;
	status = malloc_to_save_fd(tokens, start--, end, &save_fd);
	while (status == SUCCESS && ++start <= end)
	{
		if (tokens[start].type == WORD)
			status = strjoin_to_cmd_str(tokens, start, &cmd_str, vars_list);
		// else
		// 	status = process_redirect(tokens, start++, save_fd);
	}
	if (status == E_OPEN || status == E_AMBIGUOUS)
		set_exit_status_with_errout(tokens[start].str, status, vars_list);
	if (status != SUCCESS)
		return (clean_and_return(cmd_str, NULL, save_fd, status));
	command = split_cmd_str(cmd_str);
	if (command == NULL
		|| exec_command(command, is_pipe, vars_list) == E_SYSTEM
		|| restore_fd(save_fd) == E_SYSTEM)
		return (clean_and_return(cmd_str, command, save_fd, E_SYSTEM));
	return (clean_and_return(cmd_str, command, save_fd, SUCCESS));
}
