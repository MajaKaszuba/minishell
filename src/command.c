/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 23:57:38 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/05 00:00:33 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_builtin(char **tokens, t_shell *shell)
{
	if (ft_strncmp(tokens[0], "exit", 4) == 0 && ft_strlen(tokens[0]) == 4)
		builtin_help(tokens, shell);
	else if (ft_strncmp(tokens[0], "cd", 2) == 0 && ft_strlen(tokens[0]) == 2)
		builtin_cd(tokens);
	else if (ft_strncmp(tokens[0], "unset", 5) == 0
		&& ft_strlen(tokens[0]) == 5)
		builtin_unset(shell, tokens);
	else if (ft_strncmp(tokens[0], "export", 6) == 0
		&& ft_strlen(tokens[0]) == 6)
		builtin_export(shell, tokens, 1, 0);
	else if (ft_strncmp(tokens[0], "env", 3) == 0 && ft_strlen(tokens[0]) == 3)
		builtin_env(shell);
	else
		return (0);
	return (1);
}

void	command_help2(char **tokens, char **envp)
{
	char	*path;
	int		fddebug;

	if (ft_strchr(tokens[0], '/') && access(tokens[0], X_OK) == 0)
		path = tokens[0];
	else
		path = get_path(tokens[0]);
	if (path)
	{
		fddebug = open("debug", O_RDWR | O_APPEND, 0777);
		write(fddebug, tokens[1], 8);
		close(fddebug);
		execve(path, tokens, envp);
		perror("execve");
	}
	else
	{
		write(2, "Command not found: ", 19);
		write(2, tokens[0], ft_strlen(tokens[0]));
		write(2, "\n", 1);
	}
}

void	command_help(char **tokens, char **envp)
{
	if (handle_redirections(tokens) == -1)
	{
		write(2, "redirection error\n", 18);
		free_tokens(tokens);
		exit(EXIT_FAILURE);
	}
	command_help2(tokens, envp);
	free_tokens(tokens);
	exit(127);
}

void	handle_command(char **tokens, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		write(2, "fork error\n", 11);
		return ;
	}
	if (pid == 0)
		command_help(tokens, envp);
	waitpid(pid, &g_exit_status, 0);
	if (WIFEXITED(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
}

void	handle_single_command(t_shell *shell, char *input)
{
	char	**tokens;

	tokens = ft_split(input, ' ');
	free(input);
	if (!tokens[0])
		return (free_tokens(tokens));
	if (ft_strchr(tokens[1], '"') || ft_strchr(tokens[1], '\''))
	{
		handle_bunnies(tokens, '\'', 0);
		handle_bunnies(tokens, '"', 1);
	}
	else
		are_we_rich(tokens);
	if (!handle_builtin(tokens, shell))
		handle_command(tokens, shell->envp);
	free_tokens(tokens);
}
