/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:07 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/07 18:01:11 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
			path = get_path(tokens[0]);
			if (!path)
				printf("\"%s\"?? What's that sweetie \U0001F633\n", tokens[0]);
			else
			{
				pid = fork();
				if (pid == 0)
					execve(path, tokens, NULL);
				else
					waitpid(pid, NULL, 0);
				free(path);
			}
		}

		free_tokens(tokens); // Zawsze zwalniamy pamięć po iteracji
		tokens = NULL;
	}
	return (0);
}
