/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 18:33:43 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/31 14:50:19 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_nbr_before_redirection(char *line, int i)
{
	if (i == 0)
		return (0);
	i--;
	if (ft_isdigit(line[i]) == 0)
		return (0);
	while (ft_isdigit(line[i]) == 1)
	{
		i--;
		if (i == -1)
			return (1);
	}
	if (line[i] == ' ' || line[i] == '\t')
		return (1);
	else
		return (0);
}

static int	add_index_until_word_end(char *line, int *i)
{
	char	quote;

	while (ft_strchr("\n \t;<>|", line[*i]) == NULL)
	{
		if (line[*i] == '\"' || line[*i] == '\'')
		{
			quote = line[(*i)++];
			while (line[*i] != quote)
			{
				if (line[*i] == '\0')
					return (ERROR);
				(*i)++;
			}
		}
		(*i)++;
	}
	return (SUCCESS);
}

static int
	store_word_start_indexes(char *line, int **word_start_array, int *argc)
{
	int		i;

	i = 0;
	while (line[i] != '\n')
	{
		if (line[i] == ' ' || line[i] == '\t')
			i++;
		else if (ft_strchr(";<>|", line[i]) != NULL)
		{
			(*word_start_array)[(*argc)++] = i;
			if (line[i] == '>' && is_nbr_before_redirection(line, i) == 1)
				(*argc)--;
			i++;
			if (line[i - 1] == '>' && line[i] == '>')
				i++;
		}
		else
		{
			(*word_start_array)[(*argc)++] = i;
			if (add_index_until_word_end(line, &i) == ERROR)
				return (ERROR);
		}
	}
	(*word_start_array)[*argc] = -1;
	return (SUCCESS);
}

static int
	substr_from_line(char *line, int *word_start_array, char ***argv, int argc)
{
	int		i;
	int		start;
	int		next_start;

	*argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (*argv == NULL)
		return (ERROR);
	i = 0;
	while (word_start_array[i] != -1)
	{
		start = word_start_array[i];
		next_start = word_start_array[i + 1];
		if (next_start == -1)
			next_start = ft_strlen(line) - 1;
		(*argv)[i] = ft_substr(line, start, next_start - start);
		if ((*argv)[i] == NULL)
		{
			free_double_pointer(*argv);
			return (ERROR);
		}
		i++;
	}
	(*argv)[i] = NULL;
	return (SUCCESS);
}

int	split_line_into_words(char *line, char ***argv, int *argc)
{
	int		*word_start_array;
	int		status;

	word_start_array = (int *)malloc(sizeof(int) * (ft_strlen(line) + 1));
	if (word_start_array == NULL)
		return (ERROR);
	*argc = 0;
	status = store_word_start_indexes(line, &word_start_array, &argc);
	if (status == ERROR)
		return (free_and_return(word_start_array, ERROR));
	status = substr_from_line(line, word_start_array, argv, argc);
	if (status == ERROR)
		free_and_return(word_start_array, ERROR);
	return (free_and_return(word_start_array, SUCCESS));
}
