/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_token.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 11:54:50 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/11 10:32:08 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*strjoin_with_free(char *str, char *sub)
{
	char	*new_str;

	if (str == NULL)
		new_str = sub;
	else
	{
		new_str = ft_strjoin(str, sub);
		free(str);
		free(sub);
		if (new_str == NULL)
			return (NULL);
	}
	return (new_str);
}

static char	*substr_with_expand(
	char *substr_start, int len, t_str_type type, t_list *env_list)
{
	char	*sub;
	char	*tmp;

	sub = ft_substr(substr_start, 0, len);
	if (sub == NULL)
		return (NULL);
	if (type != S_QUOTE && sub[0] == '$' && sub[1] != '\0')
	{
		tmp = get_env_from_env_list(env_list, &sub[1]);
		free(sub);
		if (tmp == NULL)
			sub = ft_strdup("");
		else
			sub = ft_strdup(tmp);
		if (sub == NULL)
			return (NULL);
	}
	return (sub);
}

static char	judge_str_type(char *word, int i, t_str_type type, int *start)
{
	static t_str_type	next_type = '\0';

	if (next_type != '\0')
	{
		type = next_type;
		next_type = '\0';
	}
	if (type == RAW && word[i] == '\'' && ft_strchr(&word[i + 1], '\''))
	{
		type = S_QUOTE;
		if (i > 0 && word[i - 1] == '$')
			(*start)++;
	}
	else if (type == RAW && word[i] == '\"' && ft_strchr(&word[i + 1], '\"'))
	{
		type = D_QUOTE;
		if (i > 0 && word[i - 1] == '$')
			(*start)++;
	}
	else if ((type == S_QUOTE && word[i] == '\'')
		|| (type == D_QUOTE && word[i] == '\"'))
	{
		next_type = RAW;
	}
	return (type);
}

static int	loop_substr_and_strjoin_to_str(
	char *word, t_list *env_list, int start, char **str)
{
	int			i;
	t_str_type	type;
	char		*sub;

	i = -1;
	type = RAW;
	while (word[start] != '\0' && (++i || 1))
	{
		type = judge_str_type(word, i, type, &start);
		if ((type == RAW && ft_strchr("$\0", word[i]) != NULL)
			|| (type == S_QUOTE && ft_strchr("\'\0", word[i]) != NULL)
			|| (type == D_QUOTE && ft_strchr("\" $\0", word[i]) != NULL))
		{
			sub = substr_with_expand(&word[start], i - start, type, env_list);
			if (sub == NULL)
				return (ENOMEM);
			*str = strjoin_with_free(*str, sub);
			if (*str == NULL)
				return (ENOMEM);
			if (type == RAW && (*str)[0] == '\0')
				free_and_fill_null(str);
			start = i + (word[i] == '\"' || word[i] == '\'');
		}
	}
	return (SUCCESS);
}

int	expand_word_token(char **word, t_list *env_list)
{
	int		start;
	char	*str;
	int		status;

	start = 0;
	str = NULL;
	status = loop_substr_and_strjoin_to_str(*word, env_list, start, &str);
	if (status == ENOMEM)
		return (ENOMEM);
	free(*word);
	*word = str;
	return (SUCCESS);
}
