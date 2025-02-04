/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 23:48:12 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/04 23:59:33 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		new_env[*j] = ft_strjoin(name, "=");
		new_env[*j] = ft_strjoin(new_env[*j], value);
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

void	builtin_help2(char **tokens, t_shell *shell)
{
	write(1, "\033[38;2;255;105;180mBye Bitch ;*\033[0m\n", 37);
	free_tokens(tokens);
	free_custom_env(shell->custom_env);
	clear_history();
	exit (g_exit_status);
}

void	builtin_help(char **tokens, t_shell *shell)
{
	int		is_num;
	char	*arg;

	is_num = 1;
	if (tokens[1])
	{
		arg = tokens[1];
		while (*arg)
		{
			if (ft_isdigit(*arg) == 0)
			{
				is_num = 0;
				break ;
			}
			arg++;
		}
	}
	if (!tokens[1] || is_num)
		builtin_help2(tokens, shell);
	else
		write(1, "Invalid exit argument\n", 22);
}
