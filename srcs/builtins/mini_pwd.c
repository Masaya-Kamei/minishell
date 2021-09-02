/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 20:11:20 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/02 14:12:46 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit_status	mini_pwd(t_data *d, char **argv)
{
	char	*absolute_path;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP, P_PWD));
	if (d->pwd != NULL)
		ft_putendl_fd(d->pwd, 1);
	else
	{
		absolute_path = getcwd(NULL, 0);
		if (absolute_path == NULL)
			return (get_exit_status_with_errout(GETCWD_EMSG, E_GETCWD, P_PWD));
		ft_putendl_fd(absolute_path, 1);
		free(absolute_path);
	}
	return (0);
}
