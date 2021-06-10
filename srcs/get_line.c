/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 14:19:21 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/10 19:30:34 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_line(char **line)
{
	char	buf[BUFFER_SIZE + 1];
	char	*tmp;
	int		readsize;

	*line = ft_strdup("");
	if (*line == NULL)
		return (ERR_MALLOC);
	buf[0] = '\0';
	readsize = 1;
	while (buf[readsize - 1] != '\n')
	{
		readsize = read(0, buf, BUFFER_SIZE);
		if (readsize == -1)
			return (free_and_return(*line, ERR_READ));
		buf[readsize] = '\0';
		tmp = ft_strjoin(*line, buf);
		free(*line);
		*line = tmp;
		if (tmp == NULL)
			return (ERR_MALLOC);
	}
	return (SUCCESS);
}
