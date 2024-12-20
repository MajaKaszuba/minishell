/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:07 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/20 14:24:58 by olaf             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*path;
	char	*input;
	char	**tokens;
	char	**commands;
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	t_shell	shell;
	int		i;

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
			prev_fd = 0; // Brak poprzedniego potoku na początku
			i = 0;
			while (commands[i])
			{
				if (pipe(fd) == -1) // Tworzymy potok
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
				else if (pid == 0) // Proces potomny
				{
					if (prev_fd != 0) // Jeśli istnieje poprzedni potok, przekieruj wejście
					{
						dup2(prev_fd, STDIN_FILENO);
						close(prev_fd);
					}
					if (commands[i + 1]) // Jeśli jest następne polecenie, ustaw wyjście do potoku
					{
						dup2(fd[1], STDOUT_FILENO);
						close(fd[1]);
					}
					close(fd[0]); // Zamykamy niepotrzebne końcówki
					tokens = ft_split(commands[i], ' '); // Tokenizacja bieżącego polecenia
					path = get_path(tokens[0]); // Znajdujemy ścieżkę
					if (path)
					{
						execve(path, tokens, envp); // Wykonujemy polecenie
						perror("execve");
						free_tokens(tokens);
						exit(errno);
					}
					else
					{
						write(2, "Command not found: ", 19);
						write(2, tokens[0], ft_strlen(tokens[0]));
						write(2, "\n", 1);
						exit(127);
					}
				}
				else // Proces macierzysty
				{
					waitpid(pid, NULL, 0); // Czekamy na proces potomny
					close(fd[1]); // Zamykamy końcówkę zapisu w macierzystym
					if (prev_fd != 0)
						close(prev_fd); // Zamykamy poprzedni potok
					prev_fd = fd[0]; // Przechowujemy obecny potok dla następnego polecenia
				}
				i++;
			}
			close(prev_fd); // Zamykamy ostatni potok
			free_tokens(commands); // Zwalniamy pamięć
			free(input);
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
			if (ft_strncmp(tokens[0], "exit", 4) == 0 && ft_strlen(tokens[0]) == 4)
			{
				write(1, "\033[38;2;255;105;180mBye Bitch ;*\033[0m\n", 37);
				free_tokens(tokens);
				break;
			}
			else if (ft_strncmp(tokens[0], "cd", 2) == 0 && ft_strlen(tokens[0]) == 2)
				builtin_cd(tokens);
			else if (ft_strncmp(tokens[0], "unset", 5) == 0 && ft_strlen(tokens[0]) == 5)
				builtin_unset(&shell, tokens);
			else if (ft_strncmp(tokens[0], "export", 6) == 0 && ft_strlen(tokens[0]) == 6)
				builtin_export(&shell, tokens);
			else if (ft_strncmp(tokens[0], "env", 3) == 0 && ft_strlen(tokens[0]) == 3)
				builtin_env(&shell);
			else
			{
				pid = fork();
				if (pid == 0)
				{
					if (handle_redirections(tokens) == -1)
					{
						write(2, "redirection error\n", 18);
						free_tokens(tokens);
						exit(EXIT_FAILURE);
					}
					path = get_path(tokens[0]);
					if (path)
					{
						execve(path, tokens, envp);
						perror("execve");
						exit(errno);
					}
					else
					{
						write(2, "Command not found: ", 19);
						write(2, tokens[0], ft_strlen(tokens[0]));
						write(2, "\n", 1);
						exit(127);
					}
				}
				else if (pid > 0)
					waitpid(pid, NULL, 0);
				else
					write(2, "fork error\n", 11);
			}
			free_tokens(tokens); // Zawsze zwalniamy pamięć po iteracji
		}
	}
	free_custom_env(shell.custom_env);
	clear_history(); // Czyści historię readline
	rl_clear_history(); // Zwalnia pamięć historii readline
	rl_free_line_state(); // Zwalnia pamięć stanu linii readline
	return (0);
}