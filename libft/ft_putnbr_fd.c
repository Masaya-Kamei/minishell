/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 11:56:26 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/27 18:32:39 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	print_nbr(unsigned int nbr, int fd)
{
	if (nbr >= 10)
		print_nbr(nbr / 10, fd);
	ft_putchar_fd(nbr % 10 + '0', fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	un;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		un = -1 * n;
	}
	else
		un = n;
	print_nbr(un, fd);
}
