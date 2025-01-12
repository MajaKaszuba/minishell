/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:15 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/01/12 18:41:10 by olaf             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	builtin_cd(char **tokens)
{
	char	*home;

	if (tokens[1]) // Jeśli podano argument
	{
		if (chdir(tokens[1]) == -1) // Próba zmiany katalogu na podany
			shell_error(ft_strjoin("cd: no such file or directory: ", tokens[1]), 1);
	}
	else // Jeśli nie podano argumentu
	{
		home = getenv("HOME");
		if (!home) // Jeśli $HOME nie jest ustawione
		{
			shell_error("cd: HOME not set", 1);
			return ;
		}	
		if (chdir(home) == -1)
			shell_error("cd: failed to change to HOME directory", 1);
	}
}

void	builtin_unset(t_shell *shell, char **tokens)
{
	int i;

	if (!tokens[1]) // Brak argumentu
	{
		write(STDERR_FILENO, "unset: not enough arguments\n", 28);
		return;
	}

	i = 0;
	while (shell->custom_env && shell->custom_env[i])
	{
		if (ft_strncmp(shell->custom_env[i], tokens[1], ft_strlen(tokens[1])) == 0 &&
			shell->custom_env[i][ft_strlen(tokens[1])] == '=')
		{
			free(shell->custom_env[i]); // Zwolnij pamięć
			shell->custom_env[i] = NULL; // Usuń zmienną
			return;
		}
		i++;
	}
}

void	builtin_export(t_shell *shell, char **tokens)
{
	char	**new_env;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (shell->custom_env && shell->custom_env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2)); // Nowa tablica z dodatkowym miejscem
	while (shell->custom_env && shell->custom_env[j])
	{
		new_env[j] = shell->custom_env[j]; // Przekopiuj istniejące zmienne
		j++;
	}
	new_env[j] = ft_strdup(tokens[1]); // Dodaj nową zmienną
	new_env[j + 1] = NULL;

	free(shell->custom_env); // Zwolnij starą tablicę
	shell->custom_env = new_env; // Ustaw nową tablicę
}

void	builtin_env(t_shell *shell)
{
	int i = 0;

	while (shell->custom_env && shell->custom_env[i])
	{
		if (shell->custom_env[i] != NULL) // Ignoruj "dziury"
			printf("%s\n", shell->custom_env[i]);
		i++;
	}
}
