/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:07 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/11/27 16:02:36 by mkaszuba         ###   ########.fr       */
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
		// Obsługa pojedynczych cudzysłowów
		single_bunny(tokens);
		double_bunny(tokens);
		
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
		free_tokens(tokens);
	}
	return (0);
}
