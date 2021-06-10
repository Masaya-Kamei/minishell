/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:30:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/10 18:07:03 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_err_msg(int err_num)
{
	const char	err_msgs[6][50] = {"", "", "",
		"Not supported multiline commands",
		"syntax error near unexpected token `;'",
		"syntax error near unexpected token `;;'"};

	printf("minishell: ");
	printf("%s\n", err_msgs[err_num]);
}

static int	loop_minishell(void)
{
	char	*line;
	t_token	*tokens;
	int		token_num;
	int		status;

	while (1)
	{
		write(1, "minishell$ ", 11);
		status = get_line(&line);
		if (status == ERR_MALLOC || status == ERR_READ)
			exit(1);
		status = lex_line(line, &tokens, &token_num);
		free(line);
		if (status == ERR_MALLOC)
			exit(1);
		else if (status == ERR_MULTILINE || token_num == 0)
		{
			if (status == ERR_MULTILINE)
				write_err_msg(ERR_MULTILINE);
			else
				free_tokens(tokens);
			continue ;
		}
		print_tokens(tokens);
		status = process_commandline(tokens, 0, token_num - 1);
		free_tokens(tokens);
		if (status == ERR_MALLOC)
			exit(1);
		else if (status != SUCCESS)
			write_err_msg(status);
	}
}

int	main(void)
{
	loop_minishell();
	return (0);
}
