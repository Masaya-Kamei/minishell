/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/30 17:55:37 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/16 14:46:58 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_line_and_word_start_array(char *line, int *word_start_array)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (line[i] != '\0')
	{
		if (i == word_start_array[j])
		{
			printf("#");
			j++;
		}
		else
			printf(" ");
		i++;
	}
	printf("%s\n", line);
}

void	print_tokens(t_token *tokens)
{
	int	i;

	printf("[ tokens ]\n");
	i = 0;
	while (tokens[i].str != NULL)
	{
		printf("%2d	type:%c, str:%s\n", i, tokens[i].type, tokens[i].str);
		i++;
	}
}

void	execve_sleep(void)
{
	char	**argv;
	pid_t	pid;
	int		status;

	argv = (char **)malloc(sizeof(char *) * 3);
	argv[0] = ft_strdup("sleep");
	argv[1] = ft_strdup("3");
	argv[2] = NULL;
	pid = fork();
	if (pid == 0)
	{
		execve("/bin/sleep", argv, NULL);
		exit(1);
	}
	else
	{
		wait(&status);
	}
	free_double_pointer(argv);
}
