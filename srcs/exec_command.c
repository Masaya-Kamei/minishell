/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:24:35 by keguchi           #+#    #+#             */
/*   Updated: 2021/07/25 15:28:53 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_builtin(char **command)
{
	if (ft_strncmp(command[0], "echo", 5) == 0
		|| ft_strncmp(command[0], "cd", 3) == 0
		|| ft_strncmp(command[0], "pwd", 4) == 0
		|| ft_strncmp(command[0], "export", 7) == 0
		|| ft_strncmp(command[0], "unset", 6) == 0
		|| ft_strncmp(command[0], "env", 4) == 0
		|| ft_strncmp(command[0], "exit", 5) == 0)
		return (1);
	else
		return (0);
}

static t_exit_status	exec_builtin_command(char **command,
	t_list *vars_list[3])
{
	if (ft_strncmp(command[0], "echo", 5) == 0)
		return (mini_echo(command, vars_list));
	else if (ft_strncmp(command[0], "cd", 3) == 0)
		return (mini_cd(command, vars_list));
	else if (ft_strncmp(command[0], "pwd", 4) == 0)
		return (mini_pwd(command, vars_list));
	else if (ft_strncmp(command[0], "export", 7) == 0)
		return (mini_export(command, vars_list));
	else if (ft_strncmp(command[0], "unset", 6) == 0)
		return (mini_unset(command, vars_list));
	else if (ft_strncmp(command[0], "env", 4) == 0)
		return (mini_env(command, vars_list));
	else if (ft_strncmp(command[0], "exit", 5) == 0)
		return (mini_exit(command, vars_list));
	return (SUCCESS);
}

static t_exit_status	exec_external_command(char **cmd, t_list *vars_list[3])
{
	int			count;
	char		*cmd_path;
	char		**envp;

	count = 0;
	cmd_path = get_command_path(cmd[0]);
	if (cmd_path == NULL)
		return (get_exit_status_with_errout(NULL, E_MALLOC, P_SHELL));
	envp = create_envp(vars_list[ENV]);
	if (envp == NULL)
		return (get_exit_status_with_errout(NULL, E_MALLOC, P_SHELL));
	if (ft_strncmp(cmd_path, "", 1) == 0)
		return (get_exit_status_with_errout(cmd[0], E_EXTERNAL, P_EXTERNAL));
	execve(cmd_path, cmd, envp);
	free(cmd_path);
	return (get_exit_status_with_errout(cmd[0], E_EXECVE, P_SHELL));
}

static t_status	set_save_fd(t_token *tokens, int ***save_fd,
	int start_index, int end_index)
{
	int	i;
	int	redirect_count;

	i = start_index;
	redirect_count = 0;
	while (i <= end_index)
	{
		if (tokens[i].type == '<' || tokens[i].type == '>'
			|| tokens[i].type == 'G' || tokens[i].type == 'L')
			redirect_count++;
		i++;
	}
	i = 0;
	*save_fd = malloc(sizeof(int *) * (redirect_count + 1));
	if (!save_fd)
		return (E_MALLOC);
	while (i < redirect_count)
	{
		(*save_fd)[i] = malloc(sizeof(int) * 2);
		if (!save_fd[i])
			return (E_MALLOC);
		i++;
	}
	(*save_fd)[i] = NULL;
	return (SUCCESS);
}

static void	restore_fd_and_free(int **save_fd, char **command)
{
	int	i;

	i = 0;
	while (save_fd[i] != NULL)
	{
		dup2(save_fd[i][1], save_fd[i][0]);
		close(save_fd[i][1]);
		i++;
	}
	free_double_pointer((void **)command);
	free_double_pointer((void **)save_fd);
}

static t_status	expand_tokens(t_token *tokens, t_list *vars_list[3],
	int start_index, int end_index)
{
	int			i;
	t_status	status;

	i = start_index;
	while (i <= end_index)
	{
		if (tokens[i].type == 'W')
			status = expand_word_token(&tokens[i].str, vars_list);
		if (status != SUCCESS)
			return (status);
		i++;
	}
	return (SUCCESS);
}

static int	simple_builtin_command(char **command, int **save_fd,
	int is_pipe, t_list *vars_list[3])
{
	t_exit_status	exit_status;

	if (is_pipe == 0 && check_builtin(command))
	{
		exit_status = exec_builtin_command(command, vars_list);
		set_exit_status(vars_list[SPECIAL], exit_status);
		restore_fd_and_free(save_fd, command);
		return (1);
	}
	else
		return (0);
}

static t_status	exec_command(char **command, int **save_fd,
	int is_pipe, t_list *vars_list[3])
{
	pid_t			pid;
	int				status;

	if (simple_builtin_command(command, save_fd, is_pipe, vars_list))
		return (SUCCESS);
	pid = fork();
	if (pid < 0)
		return (E_FORK);
	else if (pid == 0)
	{
		if (check_builtin(command))
			exit(exec_builtin_command(command, vars_list));
		else
			exit(exec_external_command(command, vars_list));
	}
	else
	{
		wait(&status);
		if ((WIFEXITED(status)))
			set_exit_status(vars_list[SPECIAL], WEXITSTATUS(status));
	}
	restore_fd_and_free(save_fd, command);
	return (SUCCESS);
}

t_status	process_command(t_token *tokens, int start_index,
	int end_index, t_list *vars_list[3])
{
	t_status		status;
	char			**command;
	int				**save_fd;
	int				is_pipe;

	command = NULL;
	is_pipe = 1;
	status = expand_tokens(tokens, vars_list, start_index, end_index);
	if (status != SUCCESS)
		return (status);
	if (isatty(0) == 1 && isatty(1) == 1)
		is_pipe = 0;
	status = set_save_fd(tokens, &save_fd, start_index, end_index);
	if (status != SUCCESS)
		return (status);
	status = redirect_list(tokens, start_index, end_index, save_fd);
	if (status != SUCCESS)
		return (status);
	status = get_command_argv(tokens, &command, start_index, end_index);
	if (status != SUCCESS)
		return (status);
	return (exec_command(command, save_fd, is_pipe, vars_list));
}
