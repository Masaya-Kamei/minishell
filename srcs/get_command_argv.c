/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_argv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 01:42:34 by keguchi           #+#    #+#             */
/*   Updated: 2021/07/30 16:59:31 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strjoin_path(char *s1, char *s2)
{
	int		count;
	int		i;
	int		j;
	char	*result;

	count = 0;
	i = 0;
	j = 0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	if (!s1 && !s2)
		return (ft_strdup(" "));
	count = ft_strlen(s1) + ft_strlen(s2);
	result = (char *)malloc((count + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (s1[j] != '\0')
		result[i++] = s1[j++];
	j = 0;
	while (s2[j] != '\0')
		result[i++] = s2[j++];
	result[i] = '\0';
	return (result);
}

static char	*add_space(t_token *tokens, int i, char *cmd, char *tmp)
{
	tmp = cmd;
	cmd = ft_strjoin_path(cmd, tokens[i].str);
	if (!cmd)
		return (NULL);
	free(tmp);
	tmp = cmd;
	cmd = ft_strjoin_path(cmd, " ");
	if (!cmd)
		return (NULL);
	free(tmp);
	return (cmd);
}

t_status	get_command_argv(t_token *tokens, char ***command,
	int start_index, int end_index)
{
	int		i;
	char	*cmd;
	char	*tmp;

	i = start_index;
	cmd = NULL;
	tmp = NULL;
	while (i <= end_index)
	{
		if (tokens[i].type == '<' || tokens[i].type == '>'
			|| tokens[i].type == 'G' || tokens[i].type == 'L')
			i += 2;
		if (tokens[i].type == 'W')
		{
			cmd = add_space(tokens, i, cmd, tmp);
			if (!cmd)
				return (E_MALLOC);
			i++;
		}
	}
	*command = ft_split(cmd, ' ');
	free(cmd);
	return (SUCCESS);
}
