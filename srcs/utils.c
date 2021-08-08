/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 10:45:21 by keguchi           #+#    #+#             */
/*   Updated: 2021/08/07 17:44:45 by mkamei           ###   ########.fr       */
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
