/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/30 17:55:37 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/03 16:08:02 by mkamei           ###   ########.fr       */
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
