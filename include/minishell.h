/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:36 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/17 16:22:46 by olaf             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h> //fork and execve
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h> //waitpid
# include <readline/history.h> //history library
# include <readline/readline.h> //readline

//parser.c
char	*get_env_value(char *token, int start, int end);
char	*expand_env_variables(char *token);
void	are_we_rich(char **tokens);
int		find_closing_quote(char **tokens, int start, char quote_type);
char	*merge_tokens(char **tokens, int start, int end, char quote_type);
void	remove_merged_tokens(char **tokens, int start, int end);
void	handle_bunnies(char **tokens, char quote_type, int expand_env);

//executor.c
char	*get_path(char *command);
void	execution(char *command, char **tokens, char **envp);
void	execute_pipes(char **commands, char **env);
void	execute_single_command(char *command, char **env);

//builtin.c
void	builtin_cd(char **tokens);
void	builtin_unset(char **tokens);
void	builtin_export(char **tokens);
void	builtin_env(char **envp);

//signals.c
void	sigint_handler(int signo);
void	setup_signal_handlers(void);

//redir.c
int	redirect_output(char *filename);
int	redirect_output_append(char *filename);
int	redirect_input(char *filename);
int	redirect_input_heredoc(char *delimiter);
int	handle_redirections(char **tokens);

//utils.c
int		validate_syntax(char **tokens);
void	free_tokens(char **tokens);
char	*ft_strjoin_char(char *s, char c);
void	shell_error(char *message, int exit_code);
int		is_valid_identifier(const char *str);

#endif
