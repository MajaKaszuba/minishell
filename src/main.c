/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:07 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/04 14:31:46 by mkaszuba         ###   ########.fr       */
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
		input = readline("\001\033[38;2;255;105;180m\002Barbie Bash \U0001F485\001\033[0m\002: ");
		if (!input)
		{
			printf("\n\033[38;2;255;105;180mBye Bitch ;*\033[0m\n");
			break;
		}
		if (*input)
			add_history(input);
		tokens = ft_split(input, ' ');
		free(input);

		if (!tokens[0])
		{
			free_tokens(tokens);
			continue;
		}
		if (!validate_syntax(tokens))
		{
			free_tokens(tokens);
			continue;
		}
		// Obsługa cudzysłowów
		handle_bunnies(tokens, '\'', 0);
		handle_bunnies(tokens, '"', 1);

		
		if (!tokens[0]) // Sprawdzenie, czy po przetwarzaniu coś zostało
		{
			free_tokens(tokens);
			continue;
		}
		if (ft_strncmp(tokens[0], "exit", 4) == 0 && ft_strlen(tokens[0]) == 4)
		{
			printf("\033[38;2;255;105;180mBye Bitch ;*\033[0m\n");
			free_tokens(tokens);
			break;
		}
		else if (ft_strncmp(tokens[0], "cd", 2) == 0 && ft_strlen(tokens[0]) == 2)
			builtin_cd(tokens);
		else if (ft_strncmp(tokens[0], "unset", 5) == 0 && ft_strlen(tokens[0]) == 5)
			builtin_unset(tokens);
		else
		{
			path = get_path(tokens[0]);
			if (!path)
				shell_error(ft_strjoin("\"", ft_strjoin(tokens[0], "\" not found")), 127);
			else
			{
				pid = fork();
				if (pid < 0)
					shell_error("fork failed", 1);
				if (pid == 0 && execve(path, tokens, NULL) == -1)
					shell_error("execution failed", 126);
				free(path);
			}
		}
		free_tokens(tokens);
	}
	return (0);
}
