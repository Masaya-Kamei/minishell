/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:30:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/17 16:11:26 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handler(int signum)
{
	g_received_signal = signum;
}

int	redisplay_prompt(void)
{
	if (g_received_signal == SIGINT)
	{
		g_received_signal = 0;
		printf("\033[%dC\033[K", (int)ft_strlen(rl_prompt) + rl_end);
		rl_redisplay();
		rl_replace_line("\3", 1);
		rl_done = 1;
	}
	else if (g_received_signal == SIGQUIT)
	{
		g_received_signal = 0;
		printf("\033[%dC\033[K", (int)ft_strlen(rl_prompt) + rl_end);
		rl_redisplay();
	}
	return (0);
}

static t_bool	check_interrupt(t_data *d, char *line)
{
	t_exit_status	exit_status;

	if (line == NULL)
	{
		exit_status = ft_atoi(get_var(d->vars_list, "?"));
		clear_shell_data(d);
		printf("\033[1A\033[11C");
		rl_redisplay();
		write(2, "exit\n", 5);
		exit(exit_status);
	}
	else if (line[0] == '\3')
	{
		free(line);
		set_exit_status(d->vars_list[SPECIAL], 1);
		return (1);
	}
	return (0);
}

static void	loop_minishell(t_data *d)
{
	char		*line;
	t_token		*tokens;
	int			token_num;
	t_status	status;

	status = SUCCESS;
	while (status == SUCCESS)
	{
		g_received_signal = 0;
		line = readline("minishell$ ");
		if (check_interrupt(d, line) == 1)
			continue ;
		if (line[0] != '\0')
			add_history(line);
		status = lex_line(line, &tokens, &token_num);
		free(line);
		if (status == E_SYSTEM)
			break ;
		if (token_num != 0)
			status = start_process(d, tokens, 0, token_num - 1);
		free_tokens(tokens);
	}
	clear_shell_data(d);
	exit(get_exit_status_with_errout(NULL, status, P_SHELL));
}

int	main(int argc, char **argv, char **envp)
{
	t_data		d;

	(void)argc;
	(void)**argv;
	if (signal(SIGINT, handler) == SIG_ERR
		|| signal(SIGQUIT, handler) == SIG_ERR)
		exit(get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL));
	rl_event_hook = &redisplay_prompt;
	d.pwd = NULL;
	d.pid_list = NULL;
	d.vars_list[SHELL] = NULL;
	d.vars_list[SPECIAL] = lstnew_with_strdup("?=0  ");
	if (d.vars_list[SPECIAL] == NULL)
		exit(get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL));
	((char *)d.vars_list[SPECIAL]->content)[3] = '\0';
	d.vars_list[ENV] = create_env_list(envp);
	if (d.vars_list[ENV] == NULL
		|| countup_shlvl_env(&d.vars_list[ENV]) == E_SYSTEM
		|| set_pwd(&d, P_SHELL, NULL) == E_SYSTEM)
	{
		clear_shell_data(&d);
		exit(get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL));
	}
	loop_minishell(&d);
	return (0);
}
