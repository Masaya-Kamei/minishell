/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:30:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/15 16:45:36 by mkamei           ###   ########.fr       */
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

static void	loop_minishell(t_list *vars_list[3])
{
	char	*line;
	t_token	*tokens;
	int		token_num;
	int		status;

	while (1)
	{
		g_received_signal = 0;
		line = readline("minishell$ ");
		if (line == NULL)
		{
			clear_vars_list(vars_list);
			printf("\033[1A\033[11C");
			rl_redisplay();
			write(2, "exit\n", 5);
			exit(0);
		}
		if (line[0] != '\0')
			add_history(line);
		status = lex_line(line, &tokens, &token_num);
		free(line);
		if (status == ERR_MALLOC)
		{
			write_shell_err(NULL, ERR_MALLOC, ORIGINAL);
			set_exit_status(vars_list[SPECIAL], 1);
			continue ;
		}
		else if (token_num == 0)
		{
			free_tokens(tokens);
			continue ;
		}
		print_tokens(tokens, vars_list);
		test_vars_list(vars_list);
		// status = process_pipeline(tokens, 0, token_num - 1);
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
		free_tokens(tokens);
		// if (status == ERR_MALLOC)
		// 	exit(1);
		// else if (status != SUCCESS)
		// 	write_err_msg(status);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_list		*vars_list[3];

	(void)argc;
	(void)**argv;
	if (signal(SIGINT, handler) == SIG_ERR)
		exit(write_shell_err(NULL, errno, ERRNO));
	if (signal(SIGQUIT, handler) == SIG_ERR)
		exit(write_shell_err(NULL, errno, ERRNO));
	rl_signal_event_hook = &redisplay_prompt;
	vars_list[SHELL] = NULL;
	vars_list[SPECIAL] = lstnew_with_strdup("?=0  ");
	if (vars_list[SPECIAL] == NULL)
		exit(write_shell_err(NULL, ERR_MALLOC, ORIGINAL));
	((char *)vars_list[SPECIAL]->content)[3] = '\0';
	vars_list[ENV] = create_env_list(envp);
	if (vars_list[ENV] == NULL
		|| set_pwd_var(vars_list, 1) == ERR_MALLOC
		|| set_oldpwd_var(vars_list, 1) == ERR_MALLOC
		|| countup_shlvl_env(&vars_list[ENV]) == ERR_MALLOC)
	{
		clear_vars_list(vars_list);
		exit(write_shell_err(NULL, ERR_MALLOC, ORIGINAL));
	}
	loop_minishell(vars_list);
	return (0);
}
