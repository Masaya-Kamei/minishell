/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 14:03:48 by mkamei            #+#    #+#             */
/*   Updated: 2021/07/11 10:00:56 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*lstnew_with_strdup(char *str)
{
	char	*content;
	t_list	*new_list;

	content = ft_strdup(str);
	if (content == NULL)
		return (NULL);
	new_list = ft_lstnew(content);
	if (new_list == NULL)
	{
		free(content);
		return (NULL);
	}
	return (new_list);
}

t_list	*create_env_list_from_envp(char **envp)
{
	int		i;
	t_list	*env_list;
	t_list	*current_list;

	env_list = lstnew_with_strdup("?=0  ");
	if (env_list == NULL)
		return (NULL);
	((char *)env_list->content)[3] = '\0';
	i = 0;
	current_list = env_list;
	while (envp[i] != NULL)
	{
		current_list->next = lstnew_with_strdup(envp[i]);
		if (current_list->next == NULL)
		{
			ft_lstclear(&env_list, free);
			return (NULL);
		}
		current_list = current_list->next;
		i++;
	}
	return (env_list);
}

char	**create_envp_from_env_list(t_list *env_list)
{
	int		i;
	int		env_num;
	char	**envp;

	env_list = env_list->next;
	env_num = ft_lstsize(env_list);
	envp = (char **)malloc(sizeof(char *) * (env_num + 1));
	if (envp == NULL)
		return (NULL);
	i = 0;
	while (env_list != NULL)
	{
		envp[i] = ft_strdup(env_list->content);
		if (envp[i] == NULL)
		{
			free_double_pointer(envp);
			return (NULL);
		}
		env_list = env_list->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

void	set_command_status_env(t_list *env_list, int command_status)
{
	int		digit_num;

	if (command_status / 100 > 0)
		digit_num = 3;
	else if (command_status / 10 > 0)
		digit_num = 2;
	else
		digit_num = 1;
	((char *)env_list->content)[2 + digit_num] = '\0';
	digit_num--;
	while (digit_num >= 0)
	{
		((char *)env_list->content)[2 + digit_num] = command_status % 10 + '0';
		command_status /= 10;
		digit_num--;
	}
}

// gcc -Wall -Werror -Wextra env.c env_list.c utils.c write_msg.c
//		builtins/mini_env.c -I ../include -I ../libft/ ../libft/libft.a

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*env_list;
// 	int		i;

// 	(void)argc;
// 	i = 0;
// 	while (envp[i] != NULL)
// 		ft_putendl_fd(envp[i++], 1);
// 	write(1, "\n", 1);
// 	env_list = create_env_list_from_envp(envp);
// 	countup_shlvl_env(env_list);
// 	delete_oldpwd_env_value(env_list);
// 	argv[0] = "env";
// 	mini_env(argv, env_list);
// 	printf("\n%s\n", get_env_from_env_list(env_list, "?"));
// 	set_command_status_env(env_list, 127);
// 	printf("%s\n", get_env_from_env_list(env_list, "?"));
// 	printf("%s\n", get_env_from_env_list(env_list, "SHELL"));
// 	set_env_in_env_list(env_list, "AAA=a");
// 	printf("%s\n", get_env_from_env_list(env_list, "AAA"));
// 	set_env_in_env_list(env_list, "AAA");
// 	printf("%s\n", get_env_from_env_list(env_list, "AAA"));
// 	set_env_in_env_list(env_list, "BBB");
// 	printf("%s\n", get_env_from_env_list(env_list, "BBB"));
// 	set_env_in_env_list(env_list, "BBB=");
// 	printf("%s\n", get_env_from_env_list(env_list, "BBB"));
// 	set_env_in_env_list(env_list, "BBB=b");
// 	printf("%s\n\n", get_env_from_env_list(env_list, "BBB"));
// 	envp = create_envp_from_env_list(env_list);
// 	i = 0;
// 	while (envp[i] != NULL)
// 		ft_putendl_fd(envp[i++], 1);
// 	return (0);
// }
