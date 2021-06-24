/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:30:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/24 19:45:37 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handler(int signum)
{
	g_received_signal = signum;
}

static void	write_err_msg(int err_num)
{
	const char	err_msgs[3][50] = {"", "", ""};

	write(2, "minishell: ", 12);
	write(2, err_msgs[err_num], ft_strlen(err_msgs[err_num]));
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

static void	loop_minishell(void)
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
			printf("\033[1A\033[11Cexit\n");
			exit(0);
		}
		if (line[0] != '\0')
			add_history(line);
		status = lex_line(line, &tokens, &token_num);
		free(line);
		if (status == ERR_MALLOC)
			exit(1);
		else if (token_num == 0)
		{
			free_tokens(tokens);
			continue ;
		}
		print_tokens(tokens);
		// status = process_pipeline(tokens, 0, token_num - 1);
		status = pipe_line(tokens, token_num - 1);
		g_received_signal = 0;
		execve_sleep();
		if (g_received_signal == SIGINT)
		{
			g_received_signal = 0;
			printf("\n");
		}
		else if (g_received_signal == SIGQUIT)
		{
			g_received_signal = 0;
			printf("Quit: 3\n");
		}
		free_tokens(tokens);
		if (status == ERR_MALLOC)
			exit(1);
		else if (status != SUCCESS)
			write_err_msg(status);
	}
}

int	main(void)
{
	if (signal(SIGINT, handler) == SIG_ERR)
	{
		write(2, "minishell: ", 12);
		strerror(errno);
		exit(1);
	}
	if (signal(SIGQUIT, handler) == SIG_ERR)
	{
		write(2, "minishell: ", 12);
		strerror(errno);
		exit(1);
	}
	rl_signal_event_hook = &redisplay_prompt;
	loop_minishell();
	return (0);
}
