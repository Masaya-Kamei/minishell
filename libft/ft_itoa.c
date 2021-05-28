/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 15:30:11 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/27 18:28:01 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_int_len(int n)
{
	unsigned int	un;
	int				n_len;

	n_len = 0;
	if (n < 0)
	{
		un = -1 * n;
		n_len++;
	}
	else
		un = n;
	while (un > 0)
	{
		n_len++;
		un = un / 10;
	}
	return (n_len);
}

static void	nbr_to_str(char *str, int i, unsigned int nbr)
{
	if (nbr < 10)
		str[i] = nbr + '0';
	else
	{
		str[i] = nbr % 10 + '0';
		nbr_to_str(str, i - 1, nbr / 10);
	}
}

char	*ft_itoa(int n)
{
	int				n_len;
	unsigned int	un;
	char			*str;

	if (n == 0)
		return (ft_strdup("0"));
	n_len = get_int_len(n);
	str = (char *)malloc((n_len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		un = -1 * n;
		nbr_to_str(str, n_len - 1, un);
	}
	else
	{
		un = n;
		nbr_to_str(str, n_len - 1, un);
	}
	str[n_len] = '\0';
	return (str);
}
