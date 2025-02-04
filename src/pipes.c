/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 23:52:49 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/04 23:53:53 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	cmd_not_found(char **tokens)
{
	write(2, "Command not found: ", 19);
	write(2, tokens[0], ft_strlen(tokens[0]));
	write(2, "\n", 1);
	free_tokens(tokens);
	exit(127);
}

static void	execute_command(char *cmd, char **envp)
{
	char	**tokens;
	char	*path;

	tokens = ft_split(cmd, ' ');
	if (handle_redirections(tokens) == -1)
	{
		write(2, "redirection error\n", 18);
		free_tokens(tokens);
		exit(EXIT_FAILURE);
	}
	handle_quotes(tokens);
	path = get_path(tokens[0]);
	if (!path)
		cmd_not_found(tokens);
	execve(path, tokens, envp);
	perror("execve");
	free_tokens(tokens);
	exit(127);
}

static void	handle_child(char *cmd, char **envp, int prev_fd, int *fd)
{
	if (prev_fd != 0)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (fd[1] != -1)
		dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	execute_command(cmd, envp);
}

static int	fork_and_pipe(char **commands, char **envp, int *prev_fd, int i)
{
	int		fd[2];
	pid_t	pid;

	if (commands[i + 1] && pipe(fd) == -1)
		return (write(2, "pipe error\n", 11), -1);
	pid = fork();
	if (pid < 0)
		return (write(2, "fork error\n", 11), -1);
	if (pid == 0)
		handle_child(commands[i], envp, *prev_fd,
			fd);
	else
	{
		waitpid(pid, &g_exit_status, 0);
		if (WIFEXITED(g_exit_status))
			g_exit_status = WEXITSTATUS(g_exit_status);
		close(fd[1]);
		if (*prev_fd != 0)
			close(*prev_fd);
		*prev_fd = fd[0];
	}
	return (0);
}

void	handle_pipes(char **commands, char **envp, char **tokens, int i)
{
	int		prev_fd;

	(void)tokens;
	(void)i;
	prev_fd = 0;
	i = 0;
	while (commands[i])
	{
		if (fork_and_pipe(commands, envp, &prev_fd, i) == -1)
			break ;
		i++;
	}
	if (prev_fd != 0)
		close(prev_fd);
}
