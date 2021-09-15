/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 11:54:50 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/09 12:23:04 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_status	add_to_expand_list(char *substr,
				t_bool split_flag, t_list *vars_list[3], t_list **expand_list);

static t_bool	is_special_quote(char *word, int i, t_str_type type)
{
	if (type == RAW
		&& ((word[i] == '\'' && ft_strchr(&word[i + 1], '\''))
			|| (word[i] == '\"' && ft_strchr(&word[i + 1], '\"'))))
	{
		return (1);
	}
	else if ((type == '\'' && word[i] == '\'')
		|| (type == '\"' && word[i] == '\"'))
	{
		return (1);
	}
	return (0);
}

static t_bool	is_special_dollar(
	char *word, int i, t_str_type type, int *var_len)
{
	if (word[i] == '$' && type != '\'' && word[i + 1] != '\0'
		&& (ft_isalpha(word[i + 1]) || ft_strchr("?_", word[i + 1])))
	{
		*var_len = 1;
		if (word[i + *var_len] != '?')
		{
			while (ft_isalnum(word[i + *var_len + 1])
				|| word[i + *var_len + 1] == '_')
			{
				(*var_len)++;
			}
		}
		return (1);
	}
	return (0);
}

static t_str_type	judge_str_type(
	char *word, int i, t_expand_flag flag, t_str_type type)
{
	if (flag & EXPAND_QUOTE
		&& is_special_quote(word, i, type) == 1)
	{
		if (type == '\'' || type == '\"')
			type = RAW;
		else
			type = word[i];
		ft_strlcpy(&word[i], &word[i + 1], ft_strlen(&word[i + 1]) + 1);
		return (judge_str_type(word, i, flag, type));
	}
	else if (flag & EXPAND_QUOTE
		&& word[i] == '$' && type == RAW
		&& is_special_quote(word, i + 1, type) == 1)
	{
		ft_strlcpy(&word[i], &word[i + 1], ft_strlen(&word[i + 1]) + 1);
		return (judge_str_type(word, i, flag, type));
	}
	return (type);
}

static t_status	expand_word(char *word,
	t_list *vars_list[3], t_expand_flag flag, t_list **expand_list)
{
	int			i;
	int			start;
	t_str_type	type;
	int			var_len;

	i = -1;
	start = 0;
	type = RAW;
	while ((i == -1 || word[i] != '\0') && word[++i] != '\0')
	{
		type = judge_str_type(word, i, flag, type);
		if (flag & EXPAND_VAR && is_special_dollar(word, i, type, &var_len))
		{
			if (add_to_expand_list(ft_substr(word, start, i - start)
					, 0, NULL, expand_list) == E_SYSTEM
				|| add_to_expand_list(ft_substr(word, i, var_len + 1)
					, (flag & EXPAND_SPLIT) * (type == RAW)
					, vars_list, expand_list) == E_SYSTEM)
				return (E_SYSTEM);
			i += var_len;
			start = i + 1;
		}
	}
	return (add_to_expand_list(
			ft_substr(word, start, i - start), 0, NULL, expand_list));
}

t_status	expand_word_token(t_token word_token,
	t_list *vars_list[3], t_expand_flag flag, t_list **expand_list)
{
	t_list		*head;
	char		*copy_word;
	t_status	status;
	t_list		*last;

	copy_word = ft_strdup(word_token.str);
	if (copy_word == NULL)
		return (E_SYSTEM);
	*expand_list = lstnew_with_strdup("");
	if (*expand_list == NULL)
		return (free_and_return(copy_word, E_SYSTEM));
	head = *expand_list;
	status = expand_word(copy_word, vars_list, flag, expand_list);
	free(copy_word);
	*expand_list = head;
	if (status == E_SYSTEM)
	{
		ft_lstclear(expand_list, free);
		return (E_SYSTEM);
	}
	last = ft_lstlast(*expand_list);
	if (((char *)last->content)[0] == '\0'
		&& !(last == *expand_list && word_token.type == WORD_QUOTE))
		delete_last_list(expand_list);
	return (SUCCESS);
}
