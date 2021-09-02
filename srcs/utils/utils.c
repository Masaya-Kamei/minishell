/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 10:45:21 by keguchi           #+#    #+#             */
/*   Updated: 2021/08/27 16:41:12 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strjoin_with_null_support(char *s1, char *s2)
{
	if (s1 == NULL && s2 == NULL)
		return (ft_strdup(""));
	else if (s1 == NULL)
		return (ft_strdup(s2));
	else if (s2 == NULL)
		return (ft_strdup(s1));
	return (ft_strjoin(s1, s2));
}

t_bool	is_redirect_token(t_token token)
{
	if (token.type == GREATER || token.type == D_GREATER
		|| token.type == LESS || token.type == D_LESS)
		return (1);
	else
		return (0);
}

char	*strjoin_three(char *s1, char *s2, char *s3)
{
	char	*joined_str;
	char	*tmp;

	joined_str = ft_strjoin(s1, s2);
	if (joined_str == NULL)
		return (NULL);
	tmp = joined_str;
	joined_str = ft_strjoin(tmp, s3);
	free(tmp);
	if (joined_str == NULL)
		return (NULL);
	return (joined_str);
}
