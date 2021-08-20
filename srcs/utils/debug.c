/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/30 17:55:37 by mkamei            #+#    #+#             */
/*   Updated: 2021/08/20 14:44:11 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_line_and_word_start_array(char *line, int *word_start_array)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (line[i] != '\0')
	{
		if (i == word_start_array[j])
		{
			printf("#");
			j++;
		}
		else
			printf(" ");
		i++;
	}
	printf("%s\n", line);
}

void	print_tokens(t_token *tokens, t_list *vars_list[3])
{
	int		i;
	char	*expanded_str;

	printf("[ tokens ]\n");
	i = 0;
	while (tokens[i].type != '\0')
	{
		if (tokens[i].type == WORD)
		{
			if (expand_word_token(tokens[i].str, vars_list, 0, &expanded_str)
				 == E_SYSTEM)
				return ;
		}
		printf("%2d	type:%c, str:%s,\n", i, tokens[i].type, expanded_str);
		free(expanded_str);
		i++;
	}
}

void	execve_sleep(void)
{
	char	**argv;
	pid_t	pid;
	int		status;

	argv = (char **)malloc(sizeof(char *) * 3);
	argv[0] = ft_strdup("sleep");
	argv[1] = ft_strdup("3");
	argv[2] = NULL;
	pid = fork();
	if (pid == 0)
	{
		execve("/bin/sleep", argv, NULL);
		exit(1);
	}
	else
	{
		wait(&status);
	}
	free_double_pointer((void **)argv);
}

void	test_vars_list(t_list *vars_list[3])
{
	int		i;
	t_list	*current_list;

	i = 0;
	while (i < 3)
	{
		current_list = vars_list[i++];
		while (current_list != NULL)
		{
			printf("%s\n", (char *)current_list->content);
			current_list = current_list->next;
		}
	}
	set_exit_status(vars_list[SPECIAL], 127);
	printf("\n%s\n", get_var(vars_list, "?"));
	set_var(vars_list, "AAA", ENV);
	printf("%s\n", get_var(vars_list, "AAA"));
	set_var(vars_list, "AAA=", ENV);
	printf("%s\n", get_var(vars_list, "AAA"));
	set_var(vars_list, "AAA=b", ENV);
	printf("%s\n", get_var(vars_list, "AAA"));
	set_var(vars_list, "AAA", ENV);
	printf("%s\n", get_var(vars_list, "AAA"));
	delete_var(vars_list, "AAA", SHELL);
	printf("%s\n", get_var(vars_list, "AAA"));
}

t_status	debug_process_command(
	t_token *tokens, int start, int end, t_list *vars_list[3])
{
	char	*argv[10];
	char	*path;
	int		i;
	int		j;
	pid_t	pid;

	(void)(**vars_list);
	i = start;
	j = 0;
	while (i <= end)
		argv[j++] = tokens[i++].str;
	argv[j] = NULL;
	pid = fork();
	if (pid == 0)
	{
		if (ft_strncmp(tokens[start].str, "grep", 5) == 0)
			path = "/usr/bin/grep";
		else
			path = "/bin/cat";
		execve(path, argv, NULL);
		exit(0);
	}
	wait(NULL);
	return (SUCCESS);
}
