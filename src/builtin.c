/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:15 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/07 18:00:31 by mkaszuba         ###   ########.fr       */
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

void	builtin_unset(char **tokens)
{
	if (!tokens[1])
	{
		shell_error("unset: not enough arguments", 1);
		return ;
	}
	if (unsetenv(tokens[1]) == -1) // Usuwanie zmiennej środowiskowej
	{
		shell_error(ft_strjoin("unset: failed to unset: ", tokens[1]), 1);
	}
}

void	builtin_export(char **tokens)
{
	int		i;
	char	*name;
	char	*value;
	char	*equal_sign;

	i = 1; // Bo pomijamy zerowy token(export)
	while(tokens[i])
	{
		equal_sign = ft_strchr(tokens[i], '=');
		if (equal_sign) //Znalazło = więc mamy format VAR=value
		{
			equal_sign = "\0"; //Rozdziela na nazwe i wartość tworząc dwa ciągi
			name = tokens[i];
			value = equal_sign + 1;
			if (is_valid_identifier(name))
				setenv(name, value, 1);
			else
				shell_error(ft_strjoin("export1: not a valid identifier: ", tokens[i]), 1);
		}
		else //Nie ma = więc jest samo name
		{
			if (is_valid_identifier(tokens[i]))
				setenv(tokens[i], "", 1); //Nie ma żadnej wartości więc puste
			else
				shell_error(ft_strjoin("export2: not a valid identifier: ", tokens[i]), 1);
		}
		i++;
	}
}
