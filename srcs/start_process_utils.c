/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 15:31:42 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/10 15:36:32 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_command_exit_status(t_list *vars_list[3], int status)
{
	t_exit_status	exit_status;

	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_status = WTERMSIG(status) + 128;
	else
		exit_status = get_exit_status_with_errout(NULL, E_SYSTEM, P_SHELL);
	set_exit_status(vars_list[SPECIAL], exit_status);
}

t_status	wait_child_process(t_list **pid_list, t_list *vars_list[3])
{
	t_list	*current_list;
	int		status;
	t_bool	write_flag;

	write_flag = 0;
	current_list = *pid_list;
	while (current_list != NULL)
	{
		if (waitpid(*(pid_t *)current_list->content, &status, 0) == -1)
			return (E_SYSTEM);
		if (write_flag == 0 && WIFSIGNALED(status))
		{
			g_received_signal = 0;
			write_flag = 1;
			if (WTERMSIG(status) == SIGINT)
				write(1, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				write(1, "Quit: 3\n", 9);
		}
		current_list = current_list->next;
	}
	set_command_exit_status(vars_list, status);
	ft_lstclear(pid_list, free);
	*pid_list = NULL;
	return (SUCCESS);
}
