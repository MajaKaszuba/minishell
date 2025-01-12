/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:07 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/01/12 18:58:48 by olaf             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_exit_status = 0;

char *remove_quotes(char *str)
{
	char *result = malloc(ft_strlen(str) + 1);
	int i = 0;
	int j = 0;

	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			j++;
		i++;
	}
	result = malloc(j + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

void handle_quotes(char **tokens)
{
	int i = 0;
	char *new_str;

	while (tokens[i])
	{
		new_str = remove_quotes(tokens[i]);
		free(tokens[i]);
		tokens[i] = new_str;
		i++;
	}
}

static void handle_pipes(char **commands, char **envp)
{
	int fd[2];
	int prev_fd = 0;
	pid_t pid;
	int i = 0;
	char **tokens;
	char *path;

	while (commands[i])
	{
		if (commands[i + 1] && pipe(fd) == -1) // Tworzymy potok
		{
			write(2, "pipe error\n", 11);
			break;
		}
		pid = fork();
		if (pid == -1)
		{
			write(2, "fork error\n", 11);
			break;
		}
		if (pid == 0) // Proces potomny
		{
			if (prev_fd != 0)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (commands[i + 1])
				dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			tokens = ft_split(commands[i], ' ');
			handle_quotes(tokens);
			path = get_path(tokens[0]);
			if (path)
			{
				int fdDEBUG = open("debug", O_RDWR | O_APPEND, 0777);
				write(fdDEBUG, tokens[1], 20);
				close(fdDEBUG);
				execve(path, tokens, envp);
				perror("execve");
			}
			else
			{
				write(2, "Command not found: ", 19);
				write(2, tokens[0], ft_strlen(tokens[0]));
				write(2, "\n", 1);
			}
			free_tokens(tokens);
			exit(127);
		}
		else // Proces macierzysty
		{
			waitpid(pid, &g_exit_status, 0);
			if (WIFEXITED(g_exit_status))
			{
				g_exit_status = WEXITSTATUS(g_exit_status);
			}
			close(fd[1]);
			if (prev_fd != 0)
				close(prev_fd);
			prev_fd = fd[0];
		}
		i++;
	}
	if (prev_fd != 0)
		close(prev_fd);
}

static int handle_builtin(char **tokens, t_shell *shell)
{
	if (ft_strncmp(tokens[0], "exit", 4) == 0 && ft_strlen(tokens[0]) == 4)
	{
		write(1, "\033[38;2;255;105;180mBye Bitch ;*\033[0m\n", 37);
		free_tokens(tokens);
		free_custom_env(shell->custom_env);
		clear_history();
		exit(g_exit_status); // Zakończ z ostatnim kodem wyjścia
	}
	else if (ft_strncmp(tokens[0], "cd", 2) == 0 && ft_strlen(tokens[0]) == 2)
	{
		builtin_cd(tokens);
		g_exit_status = 0; // Sukces
	}
	else if (ft_strncmp(tokens[0], "unset", 5) == 0 && ft_strlen(tokens[0]) == 5)
	{
		builtin_unset(shell, tokens);
		g_exit_status = 0; // Sukces
	}
	else if (ft_strncmp(tokens[0], "export", 6) == 0 && ft_strlen(tokens[0]) == 6)
	{
		builtin_export(shell, tokens);
		g_exit_status = 0; // Sukces
	}
	else if (ft_strncmp(tokens[0], "env", 3) == 0 && ft_strlen(tokens[0]) == 3)
	{
		builtin_env(shell);
		g_exit_status = 0; // Sukces
	}
	else
		return (0); // Nie znaleziono polecenia wbudowanego
	return (1); // Polecenie wbudowane zostało obsłużone
}

static void handle_command(char **tokens, char **envp)
{
	pid_t pid;
	char *path;

	pid = fork();
	if (pid == -1)
	{
		write(2, "fork error\n", 11);
		return;
	}
	if (pid == 0) // Proces potomny
	{
		if (handle_redirections(tokens) == -1)
		{
			write(2, "redirection error\n", 18);
			free_tokens(tokens);
			exit(EXIT_FAILURE);
		}
		if (ft_strchr(tokens[0], '/') && access(tokens[0], X_OK) == 0)
			path = tokens[0];
		else
			path = get_path(tokens[0]);
		if (path)
		{
			int fdDEBUG = open("debug", O_RDWR | O_APPEND, 0777);
			write(fdDEBUG, tokens[1], 8);
			close(fdDEBUG);
			execve(path, tokens, envp);
			perror("execve");
		}
		else
		{
			write(2, "Command not found: ", 19);
			write(2, tokens[0], ft_strlen(tokens[0]));
			write(2, "\n", 1);
		}
		free_tokens(tokens);
		exit(127);
	}
	waitpid(pid, &g_exit_status, 0);
	if (WIFEXITED(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**tokens;
	char	**commands;
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.envp = envp; // Oryginalne środowisko
	shell.custom_env = NULL; // Customowe zmienne na początku są puste
	setup_signal_handlers(); // Ustawienie sygnałów
	shell.custom_env = init_env(envp);
	while (1)
	{
		input = readline("\001\033[38;2;255;105;180m\002Barbie Bash 💅\001\033[0m\002: ");
		if (!input) // Sprawdzenie, czy użytkownik nacisnął Ctrl+D
		{
			write(1, "\n\033[38;2;255;105;180mBye Bitch ;*\033[0m\n", 37);
			break;
		}
		if (*input) // Dodanie do historii tylko, jeśli coś podano
			add_history(input);
		// Wykrywanie `|` w poleceniach
		if (ft_strchr(input, '|'))
		{
			commands = ft_split(input, '|'); // Podział na fragmenty potoku
			handle_pipes(commands, shell.envp);
			free_tokens(commands);
		}
		else // Obsługa pojedynczych poleceń
		{
			tokens = ft_split(input, ' ');
			free(input); // Zwolnienie input

			if (!tokens[0]) // Brak polecenia
			{
				free_tokens(tokens);
				continue;
			}

			// Obsługa cudzysłowów i zmiennych
			handle_bunnies(tokens, '\'', 0);
			handle_bunnies(tokens, '"', 1);
			are_we_rich(tokens);

			// Obsługa wbudowanych poleceń
			if (handle_builtin(tokens, &shell))
			{
				free_tokens(tokens);
				continue;
			}
			else
				handle_command(tokens, shell.envp);
			free_tokens(tokens); // Zawsze zwalniamy pamięć po iteracji
		}
	}
	free_custom_env(shell.custom_env);
	clear_history(); // Czyści historię readline
	rl_clear_history(); // Zwalnia pamięć historii readline
	rl_free_line_state(); // Zwalnia pamięć stanu linii readline
	return (0);
}