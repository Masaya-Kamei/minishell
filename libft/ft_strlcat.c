/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:49:33 by mkamei            #+#    #+#             */
/*   Updated: 2020/10/18 15:10:20 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d_len;
	size_t	s_index;

	d_len = ft_strlen(dst);
	if (d_len >= size || size == 0)
		return (ft_strlen(src) + size);
	s_index = 0;
	while ((d_len + s_index) < (size - 1) && src[s_index] != '\0')
	{
		dst[d_len + s_index] = src[s_index];
		s_index++;
	}
	dst[d_len + s_index] = '\0';
	return (d_len + ft_strlen(src));
}
