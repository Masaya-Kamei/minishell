/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 18:33:43 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/03 15:22:52 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_token_end_index(char *line, int i)
{
	char	quote;
	char	*next_quote_ptr;
	t_bool	num_flag;

	if (line[i] == '|')
		return (i);
	num_flag = 1;
	while (line[i] != '\0' && ft_strchr(" <>|", line[i]) == NULL)
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote = line[i];
			next_quote_ptr = ft_strchr(&line[i + 1], quote);
			if (next_quote_ptr != NULL)
				i += next_quote_ptr - &line[i];
		}
		num_flag &= ft_isdigit(line[i]);
		i++;
	}
	i += (num_flag == 1) && (line[i] == '>' || line[i] == '<');
	i += (num_flag == 1) && ((line[i - 1] == '>' && line[i] == '>')
			|| (line[i - 1] == '<' && line[i] == '<'));
	return (i - 1);
}

static t_token_type	find_token_type(char *token_str, int end)
{
	int		i;

	if (token_str[end] == '|')
		return (PIPE);
	else if (token_str[end] == '>' && (end != 0 && token_str[end - 1] == '>'))
		return (D_GREATER);
	else if (token_str[end] == '<' && (end != 0 && token_str[end - 1] == '<'))
		return (D_LESS);
	else if (token_str[end] == '>')
		return (GREATER);
	else if (token_str[end] == '<')
		return (LESS);
	else
	{
		i = 0;
		while (i <= end)
		{
			if ((token_str[i] == '\'' && ft_strchr(&token_str[i + 1], '\''))
				|| (token_str[i] == '\"' && ft_strchr(&token_str[i + 1], '\"')))
				return (WORD_QUOTE);
			i++;
		}
		return (WORD_RAW);
	}
}

static t_status	add_to_token_list(
	char *line, int start, int len, t_list **token_list)
{
	t_token	*token;
	t_list	*new_list;

	token = (t_token *)malloc(sizeof(t_token));
	if (token == NULL)
		return (E_SYSTEM);
	token->str = ft_substr(line, start, len);
	if (token->str == NULL)
		return (free_and_return(token, E_SYSTEM));
	token->type = find_token_type(token->str, len - 1);
	new_list = ft_lstnew(token);
	if (new_list == NULL)
	{
		free(token->str);
		return (free_and_return(token, E_SYSTEM));
	}
	ft_lstadd_back(token_list, new_list);
	return (SUCCESS);
}

static t_token	*convert_token_list_to_array(t_list *token_list)
{
	t_token	*tokens;
	int		i;
	t_list	*current_list;

	tokens = (t_token *)malloc(sizeof(t_token) * (ft_lstsize(token_list) + 1));
	if (tokens == NULL)
		return (NULL);
	i = 0;
	current_list = token_list;
	while (current_list != NULL)
	{
		tokens[i++] = *(t_token *)current_list->content;
		current_list = current_list->next;
	}
	tokens[i].type = '\0';
	return (tokens);
}

t_status	lex_line(char *line, t_token **tokens, int *token_num)
{
	int			i;
	int			end;
	t_list		*token_list;

	i = -1;
	token_list = NULL;
	while (line[++i] != '\0')
	{
		if (line[i] == ' ')
			continue ;
		end = find_token_end_index(line, i);
		if (add_to_token_list(line, i, end - i + 1, &token_list) == E_SYSTEM)
		{
			ft_lstclear(&token_list, free);
			return (E_SYSTEM);
		}
		i = end;
	}
	*tokens = convert_token_list_to_array(token_list);
	*token_num = ft_lstsize(token_list);
	ft_lstclear(&token_list, free);
	if (tokens == NULL)
		return (E_SYSTEM);
	return (SUCCESS);
}
