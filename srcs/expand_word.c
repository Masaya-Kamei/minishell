/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 11:54:50 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/20 17:25:12 by mkamei           ###   ########.fr       */
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
	if (*expanded_str[0] == '\0' && substr == NULL && backup_char == '\0')
	{
		free(*expanded_str);
		*expanded_str = NULL;
		substr_start[len] = backup_char;
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

static t_bool	judge_special_dollar_char(
	char *word, int i, t_bool is_document, t_bool is_rec_call)
{
	static t_str_type	type;

	if (i == 0 && is_rec_call == 0)
		type = RAW;
	is_rec_call = 1;
	if (is_document == 1)
		is_rec_call = 0;
	else if (type == RAW
		&& ft_strchr("\'\"", word[i]) && ft_strchr(&word[i + 1], word[i]))
		type = word[i];
	else if ((type == '\'' && word[i] == '\'')
		|| (type == '\"' && word[i] == '\"'))
		type = RAW;
	else if (type == RAW && word[i] == '$'
		&& ft_strchr("\'\"", word[i + 1]) && ft_strchr(&word[i + 2], word[i + 1]))
		;
	else
		is_rec_call = 0;
	if (is_rec_call == 1)
	{
		ft_strlcpy(&word[i], &word[i + 1], ft_strlen(&word[i + 1]) + 1);
		return (judge_special_dollar_char(word, i, is_document, is_rec_call));
	}
	return (word[i] == '$' && type != '\''
		&& (ft_isalpha(word[i + 1]) || ft_strchr("?_", word[i + 1])));
}

t_status	expand_word_token(
	char *word, t_list *vars_list[3], t_bool is_document, char **expanded_str)
{
	int		i;
	int		start;
	int		dollar_index;

	i = -1;
	start = 0;
	*expanded_str = ft_strdup("");
	if (*expanded_str == NULL)
		return (E_SYSTEM);
	while (word[++i] != '\0')
	{
		if (judge_special_dollar_char(word, i, is_document, 0) == 0)
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
