/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:07 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/08 02:44:27 by olaf             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void execution(char *command, char **tokens, char **envp)
{
	if (ft_strchr(command, '/')) // Check if it's an absolute or relative path
	{
		if (access(command, X_OK) != 0) // Check for execute permissions
			shell_error(ft_strjoin("error: no access to ", command), 126);
		execve(command, tokens, envp); // Execute directly
		shell_error(ft_strjoin("error: failed to run executable ", command), 1);
		exit(errno); // Terminate child process on failure
	}
	else // If no '/' in command, use PATH-based lookup
	{
		char *path = get_path(command); // Custom function to search in PATH
		if (!path)
		{
			shell_error(ft_strjoin("error: command not found: ", command), 1);
			exit(127); // Return standard POSIX command not found error
		}
		execve(path, tokens, envp); // Execute command found in PATH
		shell_error(ft_strjoin("error: failed to run executable ", path), 1);
		free(path); // Free allocated path
		exit(errno);
	}
}

int	main(void)
{
	char	*input;
	char	**tokens;
	char	*path;
	pid_t	pid;

	while (1)
	{
		input = readline("\001\033[38;2;255;105;180m\002Barbie Bash 💅\001\033[0m\002: ");
		if (!input) // Sprawdzenie, czy użytkownik nacisnął Ctrl+D
		{
			printf("\n\033[38;2;255;105;180mBye Bitch ;*\033[0m\n");
			break;
		}
		if (*input) // Dodanie do historii tylko, jeśli coś podano
			add_history(input);
		tokens = ft_split(input, ' ');
		free(input); // Zwolnienie input

		if (!tokens[0]) // Brak polecenia
		{
			free_tokens(tokens);
			tokens = NULL;
			continue;
		}

		// Obsługa cudzysłowów i zmiennych
		handle_bunnies(tokens, '\'', 0);
		handle_bunnies(tokens, '"', 1);
		are_we_rich(tokens);


		// Jeśli po przetwarzaniu nie ma polecenia
		if (!tokens[0])
		{
			free_tokens(tokens);
			tokens = NULL;
			continue;
		}

		// Obsługa wbudowanych poleceń
		if (ft_strncmp(tokens[0], "exit", 4) == 0 && ft_strlen(tokens[0]) == 4)
		{
			printf("\033[38;2;255;105;180mBye Bitch ;*\033[0m\n");
			free_tokens(tokens);
			tokens = NULL;
			break;
		}
		else if (ft_strncmp(tokens[0], "cd", 2) == 0 && ft_strlen(tokens[0]) == 2)
			builtin_cd(tokens);
		else if (ft_strncmp(tokens[0], "unset", 5) == 0 && ft_strlen(tokens[0]) == 5)
			builtin_unset(tokens);
		else if (ft_strncmp(tokens[0], "export", 6) == 0 && ft_strlen(tokens[0]) == 6)
			builtin_export(tokens);
		else
		{
			path = get_path(tokens[0]); // Example of pre-determined path lookup
			pid = fork();
			if (pid == 0) // Child process
			{
				if (path) // Pre-determined path case
				{
					execve(path, tokens, NULL); // Direct execve
					perror("execve");
					exit(errno); // Terminate on failure
				}
				else // Use execution for dynamic lookup
				{
					execution(tokens[0], tokens, NULL);
					exit(EXIT_FAILURE); // Ensure failure exits
				}
			}
			else if (pid > 0) // Parent process
			{
				waitpid(pid, NULL, 0); // Wait for the child process to finish
			}
			else // Fork failure
			{
				perror("fork");
			}
			free(path); // Free path if allocated
		}

		free_tokens(tokens); // Zawsze zwalniamy pamięć po iteracji
		tokens = NULL;
	}
	return (0);
}
