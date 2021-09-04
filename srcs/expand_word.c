/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 11:54:50 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/04 12:09:16 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	is_special_quote(char *str, int i, t_str_type type)
{
	if (type == RAW
		&& ((str[i] == '\'' && ft_strchr(&str[i + 1], '\''))
			|| (str[i] == '\"' && ft_strchr(&str[i + 1], '\"'))))
	{
		return (1);
	}
	else if ((type == '\'' && str[i] == '\'')
		|| (type == '\"' && str[i] == '\"'))
	{
		return (1);
	}
	return (0);
}

static t_bool	is_special_dollar_with_del_quote(
	char *str, int i, t_expand_flag flag, t_bool is_rec_call)
{
	static t_str_type	type;

	if (i == 0 && is_rec_call == 0)
		type = RAW;
	if (flag & EXPAND_QUOTE
		&& is_special_quote(str, i, type) == 1)
	{
		if (type == '\'' || type == '\"')
			type = RAW;
		else
			type = str[i];
		ft_strlcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
		return (is_special_dollar_with_del_quote(str, i, flag, 1));
	}
	else if (flag & EXPAND_QUOTE
		&& str[i] == '$' && type == RAW
		&& is_special_quote(str, i + 1, type) == 1)
	{
		ft_strlcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
		return (is_special_dollar_with_del_quote(str, i, flag, 1));
	}
	return (flag & EXPAND_VAR
		&& str[i] == '$' && type != '\'' && str[i + 1] != '\0'
		&& (ft_isalpha(str[i + 1]) || ft_strchr("?_", str[i + 1])));
}

static t_status	strjoin_with_expand(
	char *substr, t_token_type type, t_list *vars_list[3], char **expanded_str)
{
	char	*expanded_substr;
	char	*tmp;

	if (substr == NULL)
		return (free_and_return(*expanded_str, E_SYSTEM));
	if (vars_list != NULL)
		expanded_substr = get_var(vars_list, substr);
	else
		expanded_substr = substr;
	tmp = *expanded_str;
	*expanded_str = strjoin_with_null_support(tmp, expanded_substr);
	free(tmp);
	free(substr);
	if (*expanded_str == NULL)
		return (E_SYSTEM);
	if (*expanded_str[0] == '\0' && type == WORD_RAW)
	{
		free(*expanded_str);
		*expanded_str = NULL;
	}
	return (SUCCESS);
}

static int	count_var_len(char *str, int start)
{
	int		var_len;

	if (str[start] == '?')
		return (1);
	var_len = 1;
	while (ft_isalnum(str[start + var_len]) || str[start + var_len] == '_')
		var_len++;
	return (var_len);
}

t_status	expand_word_token(t_token word_token,
	t_list *vars_list[3], t_expand_flag flag, char **expanded_str)
{
	int		i;
	int		start;
	char	*substr;

	i = -1;
	start = 0;
	*expanded_str = NULL;
	while ((i < 0 || word_token.str[i] != '\0') && word_token.str[++i] != '\0')
	{
		if (is_special_dollar_with_del_quote(word_token.str, i, flag, 0) == 0)
			continue ;
		substr = ft_substr(word_token.str, start, i - start);
		if (strjoin_with_expand(
				substr, word_token.type, NULL, expanded_str) == E_SYSTEM)
			return (E_SYSTEM);
		start = ++i;
		i += count_var_len(word_token.str, start);
		substr = ft_substr(word_token.str, start, i - start);
		if (strjoin_with_expand(
				substr, word_token.type, vars_list, expanded_str) == E_SYSTEM)
			return (E_SYSTEM);
		start = i--;
	}
	substr = ft_substr(word_token.str, start, i - start);
	return (strjoin_with_expand(substr, word_token.type, NULL, expanded_str));
}
