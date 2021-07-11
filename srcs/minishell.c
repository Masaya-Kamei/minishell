/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:30:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/11 10:34:37 by mkamei           ###   ########.fr       */
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

static void	loop_minishell(t_list *env_list)
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
			ft_lstclear(&env_list, free);
			printf("\033[1A\033[11Cexit\n");
			exit(0);
		}
		if (line[0] != '\0')
			add_history(line);
		status = lex_line(line, &tokens, &token_num);
		free(line);
		if (status == ENOMEM)
		{
			write_msg(NULL, NULL, ENOMEM, ERRNO);
			set_command_status_env(env_list, ENOMEM);
			continue ;
		}
		else if (token_num == 0)
		{
			free_tokens(tokens);
			continue ;
		}
		print_tokens(tokens, env_list);
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
		// if (status == ENOMEM)
		// 	exit(1);
		// else if (status != SUCCESS)
		// 	write_err_msg(status);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_list		*env_list;

	(void)argc;
	(void)**argv;
	if (signal(SIGINT, handler) == SIG_ERR)
		exit(write_msg(NULL, NULL, errno, ERRNO));
	if (signal(SIGQUIT, handler) == SIG_ERR)
		exit(write_msg(NULL, NULL, errno, ERRNO));
	rl_signal_event_hook = &redisplay_prompt;
	env_list = create_env_list_from_envp(envp);
	if (env_list == NULL)
		exit(write_msg(NULL, NULL, ENOMEM, ERRNO));
	if (delete_oldpwd_env_value(env_list) == ENOMEM)
	{
		ft_lstclear(&env_list, free);
		exit(write_msg(NULL, NULL, ENOMEM, ERRNO));
	}
	if (countup_shlvl_env(env_list) == ENOMEM)
	{
		ft_lstclear(&env_list, free);
		exit(write_msg(NULL, NULL, ENOMEM, ERRNO));
	}
	loop_minishell(env_list);
	return (0);
}
