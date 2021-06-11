/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:30:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/11 17:42:49 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_err_msg(int err_num)
{
	const char	err_msgs[3][50] = {"", "", ""};

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
		else if (token_num == 0)
		{
			free_tokens(tokens);
			continue ;
		}
		print_tokens(tokens);
		// status = process_pipeline(tokens, 0, token_num - 1);
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
