/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 14:19:21 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/31 14:50:53 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	calloc_buf_and_line(char **buf, char **line)
{
	*buf = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (*buf == NULL)
		return (ERROR);
	*line = ft_calloc(1, sizeof(char));
	if (*line == NULL)
		return (free_and_return(*buf, ERROR));
	return (SUCCESS);
}

int	get_line(char **line)
{
	char	*buf;
	char	*tmp;
	int		readsize;

	if (calloc_buf_and_line(&buf, line) == ERROR)
		return (ERROR);
	readsize = 1;
	while (buf[readsize - 1] != '\n')
	{
		readsize = read(0, buf, BUFFER_SIZE);
		if (readsize == -1)
			break ;
		buf[readsize] = '\0';
		tmp = ft_strjoin(*line, buf);
		if (tmp == NULL)
			break ;
		free(*line);
		*line = tmp;
	}
	if (buf[readsize - 1] != '\n')
	{
		free(*line);
		return (free_and_return(buf, ERROR));
	}
	return (free_and_return(buf, SUCCESS));
}
