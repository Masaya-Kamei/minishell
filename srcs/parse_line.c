/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 18:33:43 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/29 18:51:55 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_word(char *line, int *i, int **word_start, int *j)
{
	(*word_start)[(*j)++] = *i;
	while (line[*i] != '\0' && line[*i] != ' ' && line[*i] != '\t')
		(*i)++;
}

int	get_word_start_indexes(char *line, int **word_start)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	while (line[i] != '\0')
	{
		if (line[i] == ' ' || line[i] == '\t')
			i++;
		else if (line[i] == '\"' || line[i] == '\'')
		{
			(*word_start)[j++] = i;
			quote = line[i];
			while (line[++i] != quote)
				if (line[i] == '\0')
					return (MULTIPLE_LINES);
			i++;
		}
		else
			parse_word(line, &i, word_start, &j);
	}
	(*word_start)[j] = -1;
	return (SUCCESS);
}

// count と split を分ける。
// statt_index をリスト or mallocした配列(linesize) に記録していく
int	parse_line(char *line, char ***argv)
{
	int		*word_start;
	int		status;

	(void)***argv;
	word_start = (int *)malloc(sizeof(int) * ft_strlen(line));
	if (word_start == NULL)
		return (ERROR);
	status = get_word_start_indexes(line, &word_start);
	if (status == MULTIPLE_LINES)
		return (free_and_return(word_start, MULTIPLE_LINES));
	// int i = 0;
	// while (word_start[i] != -1)
	// 	printf("%d ", word_start[i++]);
	// printf("\n");
	// status = substr_from_line(line, word_start, argv);
	return (free_and_return(word_start, status));
}
