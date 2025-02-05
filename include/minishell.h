/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:36 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/05 23:08:26 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_shell
{
	char	**envp;
	char	**custom_env;
}				t_shell;

typedef struct s_bunnies
{
	char	**tokens;
	char	quote_type;
	int		expand_env;
	int		index;
	char	*merged;
}	t_bunnies;

extern int	g_exit_status;

//parser.c
void	free_custom_env(char **custom_env);
char	**init_env(char **envp);
char	*get_env_value(char *token, int start, int end);
char	*expand_env_variables(char *token);
void	are_we_rich(char **tokens);

//parser_help.c
int		find_closing_quote(char **tokens, int start, char quote_type);
char	*merge_tokens(char **tokens, int start, int end, char quote_type);
void	remove_merged_tokens(char **tokens, int start, int end);
void	handle_bunnies(char **tokens, char quote_type, int expand_env);

//executor.c
char	*get_path(char *command);
void	bunnies_help(t_bunnies *b);
void	handle_simple_quotes(
			char **tokens, char quote_type, int expand_env, int i);

//builtin.c
void	builtin_cd(char **tokens);
void	builtin_unset(t_shell *shell, char **tokens);
void	builtin_export(t_shell *shell, char **tokens, int i, int j);
char	**copy_env(char **custom_env, int j);
void	handle_export_token(char *token, char **new_env, int *j);
void	process_env_var(char *name, char *value, char **new_env, int *j);
void	add_empty_var(char *token, char **new_env, int *j);
void	builtin_env(t_shell *shell);
void	builtin_help2(char **tokens, t_shell *shell);
void	builtin_help(char **tokens, t_shell *shell);

//command.c
int		handle_builtin(char **tokens, t_shell *shell);
void	handle_command(char **tokens, char **envp);
void	handle_single_command(t_shell *shell, char *input);

//signals.c
void	sigint_handler(int signo);
void	setup_signal_handlers(void);

//redir.c
int		redirect_output(char *filename);
int		redirect_output_append(char *filename);
int		redirect_input(char *filename);
int		redirect_input_heredoc(char *delimiter);
int		handle_redirections(char **tokens);

//pipes.c
void	handle_pipes(char **commands, char **envp, char **tokens, int i);

//utils.c
int		validate_syntax(char **tokens);
void	free_tokens(char **tokens);
char	*ft_strjoin_char(char *s, char c);
void	shell_error(char *message, int exit_code);
int		is_valid_identifier(const char *str);

//utils_help.c
char	*remove_quotes(char *str);
void	handle_quotes(char **tokens);

#endif
