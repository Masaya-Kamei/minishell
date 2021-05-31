/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/30 17:55:37 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/31 13:57:21 by mkamei           ###   ########.fr       */
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
		else if (line[i] == '\n')
			printf("\n");
		else
			printf(" ");
		i++;
	}
	printf("%s\n", line);
}

void	print_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
	{
		printf("argv[%2d]	:%s\n", i, argv[i]);
		i++;
	}
}
