/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 09:24:35 by keguchi           #+#    #+#             */
/*   Updated: 2021/07/30 16:59:21 by keguchi          ###   ########.fr       */
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

static void	exec_external_command(char **cmd, t_list *vars_list[3])
{
	int			count;
	char		*cmd_path;
	char		**envp;

	count = 0;
	cmd_path = get_command_path(cmd[0]);
	if (cmd_path == NULL)
		exit(get_exit_status_with_errout(NULL, E_MALLOC, P_SHELL));
	envp = create_envp(vars_list[ENV]);
	if (envp == NULL)
		exit(get_exit_status_with_errout(NULL, E_MALLOC, P_SHELL));
	if (ft_strncmp(cmd_path, "", 1) == 0)
		exit(get_exit_status_with_errout(cmd[0], E_EXTERNAL, P_SHELL));
	execve(cmd_path, cmd, envp);
	free(cmd_path);
	exit(get_exit_status_with_errout(cmd[0], E_EXECVE, P_SHELL));
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

// static t_status	check_file_name(char *file_name)
// {
// 	if (!file_name)
// 		return (E_NULL_FILE);
// 	else if ((ft_strncmp(file_name, "", 1) == 0))
// 	{
// 		errno = 2;
// 		return (E_EMPTY_FILE);
// 	}
// 	else
// 		return (SUCCESS);
// }

// static t_status	expand_tokens(t_token *tokens, t_list *vars_list[3],
// 	int i, int end_index)
// {
// 	int			redirect_flag;
// 	char		*save_word_token;
// 	t_status	status;

// 	redirect_flag = 0;
// 	while (i <= end_index)
// 	{
// 		if (tokens[i].type == '<' || tokens[i].type == '>'
// 			|| tokens[i].type == 'G' || tokens[i].type == 'L')
// 			redirect_flag = 1;
// 		if (tokens[i].type == 'W')
// 		{
// 			save_word_token = ft_strdup(tokens[i].str);
// 			if (!save_word_token)
// 				return (E_MALLOC);
// 			status = expand_word_token(&tokens[i].str, vars_list);
// 			if (redirect_flag && status == SUCCESS)
// 				status = check_file_name(tokens[i].str);
// 			redirect_flag = 0;
// 		}
// 		free(save_word_token);
// 		if (status != SUCCESS)
// 			return (status);
// 		i++;
// 	}
// 	return (SUCCESS);
// }

static t_status	expand_tokens(t_token *tokens, t_list *vars_list[3],
	int start_index, int end_index)
{
	int			i;
	t_status	status;

	i = start_index;
	while (i <= end_index)
	{
		if (tokens[i].type == 'W')
		{
			status = expand_word_token(&tokens[i].str, vars_list);
			if (status != SUCCESS)
				return (status);
		}
		i++;
	}
	return (SUCCESS);
}

static t_status	exec_command(char **command, int is_pipe, t_list *vars_list[3])
{
	pid_t			pid;
	t_exit_status	exit_status;

	if (is_pipe && check_builtin(command))
		exit_status = exec_builtin_command(command, vars_list);
	else
	{
		pid = fork();
		if (pid < 0)
			return (E_FORK);
		else if (pid == 0)
		{
			if (check_builtin(command))
				exit(exec_builtin_command(command, vars_list));
			else
				exec_external_command(command, vars_list);
		}
		wait(&exit_status);
		if ((WIFEXITED(exit_status)))
			exit_status = WEXITSTATUS(exit_status);
		else
			exit_status = get_exit_status_with_errout(NULL, E_CHILD, P_SHELL);
	}
	return (set_exit_status(vars_list[SPECIAL], exit_status));
}

t_status	process_command(t_token *tokens, int start_index,
	int end_index, t_list *vars_list[3])
{
	t_status		status;
	char			**command;
	int				**save_fd;
	const t_bool	is_pipe = !(isatty(0) == 1 && isatty(1) == 1);

	command = NULL;
	status = expand_tokens(tokens, vars_list, start_index, end_index);
	if (status != SUCCESS)
		return (status);
	status = set_save_fd(tokens, &save_fd, start_index, end_index);
	if (status != SUCCESS)
		return (status);
	status = redirect_list(tokens, start_index, end_index, save_fd);
	if (status != SUCCESS)
		return (status);
	status = get_command_argv(tokens, &command, start_index, end_index);
	if (status != SUCCESS)
		return (status);
	status = exec_command(command, is_pipe, vars_list);
	restore_fd_and_free(save_fd, command);
	return (status);
}
