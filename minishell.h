/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:36 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/11/27 16:08:19 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h> //fork and execve
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h> //waitpid
# include <readline/history.h> //history library
# include <readline/readline.h> //readline

//main.c

//parser.c
int		are_we_rich(char **tokens);
int		find_closing_quote(char **tokens, int start, char quote_type);
char	*merge_tokens(char **tokens, int start, int end, char quote_type);
void	remove_merged_tokens(char **tokens, int start, int end);
void	single_bunny(char **tokens);

//executor.c
char	*get_path(char *command);

//builtin.c
void	builtin_cd(char **tokens);

//utils.c
void	free_tokens(char **tokens);

#endif