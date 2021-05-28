/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/08 17:36:45 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/27 18:36:31 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_start_index(char const *s1, char const *set)
{
	int		i;

	i = 0;
	while (ft_strchr(set, s1[i]) != NULL && s1[i] != '\0')
		i++;
	return (i);
}

static int	get_end_index(char const *s1, char const *set)
{
	int		i;

	i = (int)ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[i]) != NULL && s1[i] != '\0')
	{
		i--;
		if (i == -1)
			break ;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*str;

	start = get_start_index(s1, set);
	if (start == (int)ft_strlen(s1))
		return (ft_strdup(""));
	end = get_end_index(s1, set);
	str = (char *)malloc((end - start + 2) * sizeof(char));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s1 + start, end - start + 2);
	return (str);
}
