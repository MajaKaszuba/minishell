/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:15 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/04 23:48:47 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	builtin_cd(char **tokens)
{
	char	*home;

	if (tokens[1])
	{
		if (chdir(tokens[1]) == -1)
			shell_error(ft_strjoin
				("cd: no such file or directory: ", tokens[1]), 1);
	}
	else
	{
		home = getenv("HOME");
		if (!home)
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
	int	i;

	if (!tokens[1])
	{
		write(STDERR_FILENO, "unset: not enough arguments\n", 28);
		return ;
	}
	i = 0;
	while (shell->custom_env && shell->custom_env[i])
	{
		if (ft_strncmp(shell->custom_env[i], tokens[1],
				ft_strlen(tokens[1])) == 0
			&& shell->custom_env[i][ft_strlen(tokens[1])] == '=')
		{
			unsetenv(tokens[1]);
			free(shell->custom_env[i]);
			shell->custom_env[i] = NULL;
			return ;
		}
		i++;
	}
}

void	builtin_export(t_shell *shell, char **tokens, int i, int j)
{
	char	**new_env;

	i = 1;
	new_env = malloc(sizeof(char *) * (j + 2));
	if (!new_env)
		return ;
	j = 0;
	while (shell->custom_env && shell->custom_env[j])
	{
		new_env[j] = shell->custom_env[j];
		j++;
	}
	while (tokens[i])
		handle_export_token(tokens[i++], new_env, &j);
	new_env[j] = NULL;
	free(shell->custom_env);
	shell->custom_env = new_env;
}

char	**copy_env(char **custom_env, int j)
{
	char	**new_env;

	while (custom_env && custom_env[j])
		j++;
	new_env = malloc(sizeof(char *) * (j + 2));
	if (!new_env)
		return (NULL);
	j = 0;
	while (custom_env && custom_env[j])
	{
		new_env[j] = custom_env[j];
		j++;
	}
	return (new_env);
}

void	builtin_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->custom_env && shell->custom_env[i])
	{
		if (shell->custom_env[i] != NULL)
			printf("%s\n", shell->custom_env[i]);
		i++;
	}
}
