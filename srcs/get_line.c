/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 14:19:21 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/02 14:02:45 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	calloc_buf_and_line(char **buf, char **line)
{
	*buf = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (*buf == NULL)
		return (ERR_MALLOC);
	*line = ft_calloc(1, sizeof(char));
	if (*line == NULL)
		return (free_and_return(*buf, ERR_MALLOC));
	return (SUCCESS);
}

int	get_line(char **line)
{
	char	*buf;
	char	*tmp;
	int		readsize;

	if (calloc_buf_and_line(&buf, line) == ERR_MALLOC)
		return (ERR_MALLOC);
	readsize = 1;
	while (buf[readsize - 1] != '\n')
	{
		readsize = read(0, buf, BUFFER_SIZE);
		if (readsize == -1)
		{
			free(*line);
			return (free_and_return(buf, ERR_READ));
		}
		buf[readsize] = '\0';
		tmp = ft_strjoin(*line, buf);
		free(*line);
		*line = tmp;
		if (tmp == NULL)
			return (free_and_return(buf, ERR_MALLOC));
	}
	return (free_and_return(buf, SUCCESS));
}
