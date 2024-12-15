/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:19 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/15 17:43:45 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_path(char *command)
{
	int		i;
	char	*path_env;
	char	**paths;
	char	*full_path;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':'); //Dzieli path na katalogi
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, command); //Tworzy pełną ścieżkę
		if (access(full_path, X_OK) == 0) //Sprawdza,czy plik istnieje i jest wykonywalny
		{
			free_tokens(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_tokens(paths);
	return (NULL);
}

void	execution(char *command, char **tokens, char **envp)
{
	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) != 0)
			shell_error(ft_strjoin("error: no access to ", command), 126);
		execve(command, tokens, envp);
		shell_error(ft_strjoin("error: failed to run executable ", command), 1);
		exit(errno);
	}
	else
	{
		char *path = get_path(command);
		if (!path)
		{
			shell_error(ft_strjoin("error: command not found: ", command), 1);
			exit(127);
		}
		execve(path, tokens, envp);
		shell_error(ft_strjoin("error: failed to run executable ", path), 1);
		free(path);
		exit(errno);
	}
}

void	execute_pipes(char **commands, char **env)
{
	int		pipefd[2];
	pid_t	pid;
	int		in_fd; // Zmienna do przechowywania wejścia dla następnej komendy
	int		i;

	in_fd = 0;
	i = 0;
	while (commands[i])
	{
		if (commands[i + 1]) // Jeśli to nie ostatnia komenda, twórz pipe
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) // Proces dziecka
		{
			if (in_fd != 0) // Ustaw wejście, jeśli nie jest to pierwsza komenda
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			if (commands[i + 1]) // Ustaw wyjście, jeśli to nie ostatnia komenda
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
				close(pipefd[0]);
			}
			execute_single_command(commands[i], env); // Wykonanie komendy
			exit(EXIT_FAILURE); // Jeśli execve się nie powiedzie
		}
		else // Proces rodzica
		{
			waitpid(pid, NULL, 0); // Czekaj na zakończenie dziecka
			if (in_fd != 0) // Zamknij stare wejście
				close(in_fd);
			if (commands[i + 1]) // Przygotuj nowe wejście
			{
				close(pipefd[1]);
				in_fd = pipefd[0];
			}
		}
		i++;
	}
}

void	execute_single_command(char *command, char **env)
{
	char	**tokens;

	tokens = ft_split(command, ' '); // Rozdzielanie komendy na tokeny
	if (!tokens)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	execve(get_path(tokens[0]), tokens, env); // Wykonanie programu
	perror("execve"); // Jeśli execve się nie powiedzie
	free_tokens(tokens);
	exit(EXIT_FAILURE);
}
