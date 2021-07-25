/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:30:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/25 13:04:31 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handler(int signum)
{
	g_received_signal = signum;
}

static int	redisplay_prompt(void)
{
	if (g_received_signal == SIGINT)
	{
		g_received_signal = 0;
		printf("\033[%dC\033[K\n", 11 + rl_end);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (g_received_signal == SIGQUIT)
	{
		g_received_signal = 0;
		printf("\033[%dC\033[K", 11 + rl_end);
		rl_redisplay();
	}
	return (0);
}

static void	exit_by_eof(t_list *vars_list[3])
{
	clear_vars_list(vars_list);
	printf("\033[1A\033[11C");
	rl_redisplay();
	write(2, "exit\n", 5);
	exit(0);
}

static void	loop_minishell(t_list *vars_list[3])
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
		if (line == NULL)
			exit_by_eof(vars_list);
		if (line[0] != '\0')
			add_history(line);
		status = lex_line(line, &tokens, &token_num);
		free(line);
		if (status == E_MALLOC)
			break ;
		status = start_process(tokens, 0, token_num - 1, vars_list);
		free_tokens(tokens);
	}
	clear_vars_list(vars_list);
	exit(get_exit_status_with_errout(NULL, status, P_SHELL));
}
		// print_tokens(tokens, vars_list);
		// test_vars_list(vars_list);
		// g_received_signal = 0;
		// execve_sleep();
		// if (g_received_signal == SIGINT)
		// {
		// 	g_received_signal = 0;
		// 	printf("\n");
		// }
		// else if (g_received_signal == SIGQUIT)
		// {
		// 	g_received_signal = 0;
		// 	printf("Quit: 3\n");
		// }

int	main(int argc, char **argv, char **envp)
{
	t_list		*vars_list[3];

	(void)argc;
	(void)**argv;
	if (signal(SIGINT, handler) == SIG_ERR)
		exit(get_exit_status_with_errout(NULL, E_SIGNAL, P_SHELL));
	if (signal(SIGQUIT, handler) == SIG_ERR)
		exit(get_exit_status_with_errout(NULL, E_SIGNAL, P_SHELL));
	rl_signal_event_hook = &redisplay_prompt;
	vars_list[SHELL] = NULL;
	vars_list[SPECIAL] = lstnew_with_strdup("?=0  ");
	if (vars_list[SPECIAL] == NULL)
		exit(get_exit_status_with_errout(NULL, E_MALLOC, P_SHELL));
	((char *)vars_list[SPECIAL]->content)[3] = '\0';
	vars_list[ENV] = create_env_list(envp);
	if (vars_list[ENV] == NULL
		|| set_oldpwd_var(vars_list, 1) == E_MALLOC
		|| set_pwd_var(vars_list, 1) == E_MALLOC
		|| countup_shlvl_env(&vars_list[ENV]) == E_MALLOC)
	{
		clear_vars_list(vars_list);
		exit(get_exit_status_with_errout(NULL, E_MALLOC, P_SHELL));
	}
	loop_minishell(vars_list);
	return (0);
}
