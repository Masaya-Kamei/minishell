/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:30:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/03 12:03:49 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_err_msgs(char err_msgs[ERR_NUM][ERR_MSG_MAX_LEN])
{
	ft_strlcpy(err_msgs[ERR_MULTILINE],
		"Not supported multiline commands\n", 34);
}

static int	loop_minishell(char err_msgs[ERR_NUM][ERR_MSG_MAX_LEN])
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
		else if (status == ERR_MULTILINE)
		{
			printf("%s", err_msgs[ERR_MULTILINE]);
			continue ;
		}
		print_tokens(tokens);
		//status = process_commandline(tokens, 0, token_num - 1);
		free_tokens(tokens);
		// if (status == ERR_MALLOC)
		// 	exit(1);
		//else if (status != SUCCESS)
		// 	printf("%s", err_msgs[status]);
	}
}

int	main(void)
{
	char	err_msgs[ERR_NUM][ERR_MSG_MAX_LEN];

	set_err_msgs(err_msgs);
	loop_minishell(err_msgs);
	return (0);
}
