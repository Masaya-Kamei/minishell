/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_word_to_string.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 11:54:50 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/22 19:13:10 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	strjoin_str_and_sub(char **str, char *sub)
{
	char	*tmp;

	if (*str == NULL)
		*str = sub;
	else
	{
		tmp = ft_strjoin(*str, sub);
		free(*str);
		free(sub);
		*str = tmp;
		if (*str == NULL)
			return (ERR_MALLOC);
	}
	return (SUCCESS);
}

static int	substr_and_strjoin_to_str(
	char *substr_start, int len, char *str_type, char **str)
{
	char	*sub;
	char	*tmp;

	if (len == 0 && *str_type == *RAW)
		return (SUCCESS);
	sub = ft_substr(substr_start, 0, len);
	if (sub == NULL)
		return (ERR_MALLOC);
	if (*str_type != '\'' && sub[0] == '$' && sub[1] != '\0')
	{
		tmp = getenv(&sub[1]);
		free(sub);
		if (tmp == NULL && *str_type == *RAW)
			return (SUCCESS);
		else if (tmp == NULL && *str_type == '\"')
			sub = ft_strdup("");
		else
			sub = ft_strdup(tmp);
		if (sub == NULL)
			return (ERR_MALLOC);
	}
	return (strjoin_str_and_sub(str, sub));
}

static int	get_string_from_word(char *word, int i, int start, char **str)
{
	char	*str_type;

	str_type = RAW;
	while (word[i] != '\0')
	{
		if (*str_type == *RAW && (word[i] == '\'' || word[i] == '\"')
			&& ft_strchr(&word[i + 1], word[i]) != NULL)
		{
			str_type = &word[i];
			start += (i > 0 && word[i - 1] == '$');
		}
		if ((*str_type == *RAW && word[i] == '$')
			|| (*str_type == '\"' && ft_strchr("\" \t$", word[i]) != NULL)
			|| (*str_type == '\'' && word[i] == '\''))
		{
			if (substr_and_strjoin_to_str(
					&word[start], i - start, str_type, str) == ERR_MALLOC)
				return (ERR_MALLOC);
			start = i + (word[i] == '\"' || word[i] == '\'');
		}
		if (*str_type == word[i] && str_type != &word[i])
			str_type = RAW;
		i++;
	}
	return (substr_and_strjoin_to_str(&word[start], i - start, str_type, str));
}

int	change_word_to_string(char **word)
{
	int		i;
	int		start;
	char	*str;

	i = 0;
	start = 0;
	str = NULL;
	if (get_string_from_word(*word, i, start, &str) == ERR_MALLOC)
		return (free_and_return(str, ERR_MALLOC));
	free(*word);
	*word = str;
	return (SUCCESS);
}
