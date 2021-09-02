/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keguchi <keguchi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:33:51 by mkamei            #+#    #+#             */
/*   Updated: 2021/09/02 14:12:51 by keguchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exit_status	mini_unset(t_data *d, char **argv)
{
	int				i;
	int				var_name_len;
	t_exit_status	exit_status;

	if (argv[1] != NULL && argv[1][0] == '-' && argv[1][1] != '\0')
		return (get_exit_status_with_errout(argv[1], E_INVALID_OP, P_UNSET));
	i = 1;
	exit_status = 0;
	while (argv[i] != NULL)
	{
		var_name_len = ft_strlen(argv[i]);
		if (check_valid_identifier(argv[i], var_name_len) == 0)
			exit_status
				= get_exit_status_with_errout(argv[i], E_INVALID_ID, P_UNSET);
		else
			delete_var(d->vars_list, argv[i], SHELL);
		i++;
	}
	return (exit_status);
}
