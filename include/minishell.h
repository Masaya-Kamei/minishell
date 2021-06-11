/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkamei <mkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 09:39:27 by mkamei            #+#    #+#             */
/*   Updated: 2021/06/11 17:38:04 by mkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>

# define BUFFER_SIZE 10
# define SUCCESS 0
# define ERR_MALLOC 1
# define ERR_READ 2

typedef enum e_token_type{
	PIPE		= '|',
	GREATER		= '>',
	LESS		= '<',
	D_GREATER	= 'G',
	D_LESS		= 'L',
	WORD		= 'W'
}			t_token_type;
typedef struct s_token
{
	char			*str;
	t_token_type	type;
}				t_token;

int		get_line(char **line);
int		lex_line(char *line, t_token **tokens, int *token_num);
void	free_double_pointer(char **strs);
void	free_tokens(t_token *tokens);
int		free_and_return(void *p, int status);
void	print_line_and_word_start_array(char *line, int *word_start);
void	print_tokens(t_token *tokens);

#endif
