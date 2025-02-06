/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 23:57:38 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/07 00:16:13 by mkaszuba         ###   ########.fr       */
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

static void	remove_quote(char **tokens)
{
	int		i;
	char	*new_token;
	size_t	len;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '"' || tokens[i][0] == '\'')
		{
			len = ft_strlen(tokens[i]);
			new_token = ft_substr(tokens[i], 1, len - 2);
			free(tokens[i]);
			tokens[i] = new_token;
		}
		i++;
	}
}

void	command_help2(char **tokens, char **envp)
{
	char	*path;

	remove_quote(tokens);
	if (ft_strchr(tokens[0], '/') && access(tokens[0], X_OK) == 0)
		path = tokens[0];
	else
		path = get_path(tokens[0], 0, NULL);
	if (path)
	{
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
