/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:30:07 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/29 18:49:58 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_line(char **line)
{
	char	*buf;
	char	*tmp;
	int		readsize;

	buf = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (buf == NULL)
		return (free_and_return(buf, ERROR));
	readsize = 1;
	while (buf[readsize - 1] != '\n')
	{
		readsize = read(0, buf, BUFFER_SIZE);
		if (readsize == -1)
			return (free_and_return(buf, ERROR));
		buf[readsize] = '\0';
		tmp = ft_strjoin(*line, buf);
		free(*line);
		*line = tmp;
		if (*line == NULL)
			return (free_and_return(buf, ERROR));
	}
	return (free_and_return(buf, SUCCESS));
}

int	get_strs_len(char **strs)
{
	int		i;

	i = 0;
	while (strs[i] != NULL)
		i++;
	return (i);
}

int	main()
{
	char	*line;
	char	**argv;
	int		status;

	line = NULL;
	while (1)
	{
		if (line == NULL)
		{
			line = ft_strdup("");
			if (line == NULL)
				return (1);
		}
		if (line[0] == '\0')
			write(1, "minishell$ ", 11);
		else
			write(1, "> ", 2);
		status = get_line(&line);
		if (status == ERROR)
			return (free_and_return(line, 1));
		status = parse_line(line, &argv);
		if (status == MULTIPLE_LINES)
			continue ;
		else if (status == ERROR)
			return (free_and_return(line, 1));
		write(1, line, ft_strlen(line));
		free(line);
		line = NULL;
		// process_commandline(argv, 0, get_strs_len(argv));
		free_double_pointer(argv);
	}
	return (0);
}
