/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 09:39:27 by mkamei            #+#    #+#             */
/*   Updated: 2021/05/31 14:46:05 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>

# define BUFFER_SIZE 10
# define SUCCESS 0
# define ERROR 1

int		get_line(char **line);
int		split_line_into_words(char *line, char ***argv, int *argc);
void	free_double_pointer(char **strs);
int		free_and_return(void *p, int status);
void	print_line_and_word_start_array(char *line, int *word_start);
void	print_argv(char **argv);

#endif
