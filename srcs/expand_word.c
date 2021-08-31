/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 11:54:50 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/28 12:08:00 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_status	substr_and_strjoin(
	char *substr_start, int len, t_list *vars_list[3], char **expanded_str)
{
	char		*substr;
	char		*tmp;
	const char	backup_char = substr_start[len];

	if (len == 0)
		return (SUCCESS);
	substr_start[len] = '\0';
	if (vars_list != NULL)
		substr = get_var(vars_list, &substr_start[1]);
	else
		substr = substr_start;
	if ((substr == NULL || substr[0] == '\0')
		&& *expanded_str[0] == '\0' && backup_char == '\0')
	{
		substr_start[len] = backup_char;
		safe_free((void **)expanded_str);
		return (SUCCESS);
	}
	tmp = *expanded_str;
	*expanded_str = strjoin_with_null_support(tmp, substr);
	free(tmp);
	substr_start[len] = backup_char;
	if (*expanded_str == NULL)
		return (E_SYSTEM);
	return (SUCCESS);
}

t_bool	is_special_quote_char(char *word, int i, t_str_type type)
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

static t_bool	is_special_dollar_char(
	char *word, int i, t_expand_flag flag, t_bool is_rec_call)
{
	static t_str_type	type;

	if (i == 0 && is_rec_call == 0)
		type = RAW;
	if (flag & EXPAND_QUOTE
		&& is_special_quote_char(word, i, type) == 1)
	{
		if (type == '\'' || type == '\"')
			type = RAW;
		else
			type = word[i];
		ft_strlcpy(&word[i], &word[i + 1], ft_strlen(&word[i + 1]) + 1);
		return (is_special_dollar_char(word, i, flag, 1));
	}
	else if (flag & EXPAND_QUOTE && word[i] == '$' && type == RAW
		&& is_special_quote_char(word, i + 1, type) == 1)
	{
		ft_strlcpy(&word[i], &word[i + 1], ft_strlen(&word[i + 1]) + 1);
		return (is_special_dollar_char(word, i, flag, 1));
	}
	return (flag & EXPAND_VAR
		&& word[i] == '$' && type != '\'' && word[i + 1] != '\0'
		&& (ft_isalpha(word[i + 1]) || ft_strchr("?_", word[i + 1])));
}

t_status	expand_word_token(
	char *word, t_list *vars_list[3], t_expand_flag flag, char **expanded_str)
{
	int		i;
	int		start;
	int		dollar_index;

	i = -1;
	start = 0;
	*expanded_str = ft_strdup("");
	if (*expanded_str == NULL)
		return (E_SYSTEM);
	while ((i == -1 || word[i] != '\0') && word[++i] != '\0')
	{
		if (is_special_dollar_char(word, i, flag, 0) == 0)
			continue ;
		dollar_index = i++;
		if (word[dollar_index + 1] != '?')
			while (ft_isalnum(word[i + 1]) || word[i + 1] == '_')
				i++;
		if (substr_and_strjoin(&word[start]
				, dollar_index - start, NULL, expanded_str) == E_SYSTEM
			|| substr_and_strjoin(&word[dollar_index]
				, i + 1 - dollar_index, vars_list, expanded_str) == E_SYSTEM)
			return (E_SYSTEM);
		start = i + 1;
	}
	return (substr_and_strjoin(&word[start], i - start, NULL, expanded_str));
}
