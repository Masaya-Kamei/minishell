/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 18:33:43 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/11 17:57:00 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_index_until_token_end(char *line, int *i)
{
	char	quote;
	int		num_flag;

	num_flag = 1;
	while (ft_strchr("\n \t<>|", line[*i]) == NULL)
	{
		if (line[*i] == '\'' || line[*i] == '\"')
		{
			quote = line[(*i)++];
			while (line[*i] != quote)
			{
				if (line[*i] == '\n')
					return ;
				(*i)++;
			}
		}
		num_flag = num_flag && ft_isdigit(line[*i]);
		(*i)++;
	}
	*i += (num_flag == 1) && (line[*i] == '>' || line[*i] == '<');
	*i += (num_flag == 1) && ((line[*i - 1] == '>' && line[*i] == '>')
			|| (line[*i - 1] == '<' && line[*i] == '<'));
}

static void	store_in_token_start_indexes(
	char *line, int *token_start_indexes, int *token_num)
{
	int		i;

	i = 0;
	*token_num = 0;
	while (line[i] != '\n')
	{
		if (line[i] == ' ' || line[i] == '\t')
			i++;
		else if (ft_strchr("<>|", line[i]) != NULL)
		{
			token_start_indexes[(*token_num)++] = i++;
			i += (line[i - 1] == '>' && line[i] == '>')
				|| (line[i - 1] == '<' && line[i] == '<');
		}
		else
		{
			token_start_indexes[(*token_num)++] = i;
			add_index_until_token_end(line, &i);
		}
	}
	token_start_indexes[*token_num] = -1;
}

static int	store_in_str_member_of_t_token(
	char *line, int *token_start_indexes, t_token *tokens)
{
	int		i;
	int		start;
	int		next_start;

	i = 0;
	while (token_start_indexes[i] != -1)
	{
		start = token_start_indexes[i];
		next_start = token_start_indexes[i + 1];
		if (next_start == -1)
			next_start = ft_strlen(line) - 1;
		tokens[i].str = ft_substr(line, start, next_start - start);
		if (tokens[i].str == NULL)
			return (ERR_MALLOC);
		i++;
	}
	tokens[i].str = NULL;
	return (SUCCESS);
}

static void	store_in_type_member_of_t_token(t_token *tokens)
{
	int		i;
	int		j;

	i = 0;
	while (tokens[i].str != NULL)
	{
		if (tokens[i].str[0] == '|')
			tokens[i].type = '|';
		else
		{
			j = 0;
			while (ft_isdigit(tokens[i].str[j]) == 1)
				j++;
			if (tokens[i].str[j] == '>' && tokens[i].str[j + 1] == '>')
				tokens[i].type = D_GREATER;
			else if (tokens[i].str[j] == '<' && tokens[i].str[j + 1] == '<')
				tokens[i].type = D_LESS;
			else if (tokens[i].str[j] == '>' || tokens[i].str[j] == '<')
				tokens[i].type = tokens[i].str[j];
			else
				tokens[i].type = WORD;
		}
		i++;
	}
}

int	lex_line(char *line, t_token **tokens, int *token_num)
{
	int		*token_start_indexes;
	int		status;

	token_start_indexes = (int *)malloc(sizeof(int) * (ft_strlen(line) + 1));
	if (token_start_indexes == NULL)
		return (ERR_MALLOC);
	store_in_token_start_indexes(line, token_start_indexes, token_num);
	*tokens = (t_token *)malloc(sizeof(t_token) * (*token_num + 1));
	if (*tokens == NULL)
		return (free_and_return(token_start_indexes, ERR_MALLOC));
	status = store_in_str_member_of_t_token(line, token_start_indexes, *tokens);
	free(token_start_indexes);
	if (status == ERR_MALLOC)
	{
		free_tokens(*tokens);
		return (ERR_MALLOC);
	}
	store_in_type_member_of_t_token(*tokens);
	return (SUCCESS);
}
