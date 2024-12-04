/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:15 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/04 17:35:26 by olaf             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
