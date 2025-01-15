/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:15 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/01/13 16:17:19 by mkaszuba         ###   ########.fr       */
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

void	builtin_export(t_shell *shell, char **tokens, int i, int j, int k)
{
	char	*name;
	char	*value;
	char	*equal_sign;
	char	**new_env;
	char	**new_tokens;
	i = 1;
	while (shell->custom_env && shell->custom_env[j])
		j++;
	new_env = malloc(sizeof(char *) * (j + 2));
	while (shell->custom_env && shell->custom_env[k])
	{
		new_env[k] = shell->custom_env[k];
		k++;
	}
	while (tokens[i])
	{
		equal_sign = ft_strchr(tokens[i], '=');
		if (equal_sign)
		{
			new_tokens = ft_split(tokens[i], '=');
			name = new_tokens[0];
			value = new_tokens[1];
			if (is_valid_identifier(name))
			{
				setenv(name, value, 1);
				new_env[k] = ft_strdup(tokens[1]);
				new_env[k + 1] = NULL;
				free(shell->custom_env);
				shell->custom_env = new_env;
			}
			else
				shell_error(ft_strjoin
					("export: not a valid identifier: ", tokens[i]), 1);
		}
		else
		{
			if (is_valid_identifier(tokens[i]))
			{
				setenv(tokens[i], "", 1);
				new_env[k] = ft_strdup(tokens[1]);
				new_env[k + 1] = NULL;
				free(shell->custom_env);
				shell->custom_env = new_env;
			}
			else
				shell_error(ft_strjoin
					("export: not a valid identifier: ", tokens[i]), 1);
		}
		i++;
	}
}

// void	builtin_export(t_shell *shell, char **tokens)
// {
// 	char	**new_env;
// 	int		i;
// 	int		j;

// 	j = 0;
// 	k = 0;
// 	while (shell->custom_env && shell->custom_env[j])
// 		j++;
// 	new_env = malloc(sizeof(char *) * (j + 2));
// 	while (shell->custom_env && shell->custom_env[k])
// 	{
// 		new_env[k] = shell->custom_env[k];
// 		k++;
// 	}
// 	new_env[k] = ft_strdup(tokens[1]);
// 	new_env[k + 1] = NULL;
// 	free(shell->custom_env);
// 	shell->custom_env = new_env;
// }

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
