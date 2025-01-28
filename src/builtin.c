/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:15 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/01/20 15:53:00 by mkaszuba         ###   ########.fr       */
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
	new_env = copy_env(shell->custom_env, j);
	if (!new_env)
		return ;
	while (tokens[i])
	{
		handle_export_token(tokens[i], new_env, &j);
		i++;
	}
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

void	handle_export_token(char *token, char **new_env, int *j)
{
	char	**new_tokens;
	char	*name;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(token, '=');
	if (equal_sign)
	{
		new_tokens = ft_split(token, '=');
		if (!new_tokens)
			return ;
		name = new_tokens[0];
		value = new_tokens[1];
		process_env_var(name, value, new_env, j);
		free_tokens(new_tokens);
	}
	else
		add_empty_var(token, new_env, j);
}

void	process_env_var(char *name, char *value, char **new_env, int *j)
{
	if (is_valid_identifier(name))
	{
		setenv(name, value, 1);
		new_env[*j] = ft_strdup(name);
		(*j)++;
	}
	else
		shell_error(ft_strjoin("export: not a valid identifier: ", name), 1);
}

void	add_empty_var(char *token, char **new_env, int *j)
{
	if (is_valid_identifier(token))
	{
		setenv(token, "", 1);
		new_env[*j] = ft_strdup(token);
		(*j)++;
	}
	else
		shell_error(ft_strjoin("export: not a valid identifier: ", token), 1);
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
