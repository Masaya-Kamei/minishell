/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/07 17:38:26 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/27 18:20:22 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int				i;
	unsigned long	nbr;
	int				sign;

	i = 0;
	sign = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == '\n')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	nbr = 0;
	while (str[i] >= '0' && str[i] <= '9')
		nbr = (nbr * 10) + str[i++] - '0';
	if (sign == 1 && nbr > LONG_MAX)
		return (-1);
	if (sign == -1 && (nbr - 1) > LONG_MAX)
		return (0);
	return (sign * nbr);
}
