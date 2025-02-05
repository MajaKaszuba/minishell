/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:19 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/05 23:07:22 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_path(char *command)
{
	int		i;
	char	*path_env;
	char	**paths;
	char	*full_path;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, command);
		if (access(full_path, X_OK) == 0)
		{
			free_tokens(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_tokens(paths);
	return (NULL);
}

void	are_we_rich(char **tokens)
{
	int		i;
	char	*expanded;

	i = 0;
	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '$') && tokens[i][0] != '\'')
		{
			expanded = expand_env_variables(tokens[i]);
			free(tokens[i]);
			tokens[i] = expanded;
		}
		i++;
	}
}

void	handle_bunnies(char **tokens, char quote_type, int expand_env)
{
	int			i;
	t_bunnies	b;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == quote_type)
		{
			if (tokens[i][ft_strlen(tokens[i]) - 1] == quote_type)
			{
				handle_simple_quotes(tokens, quote_type, expand_env, i);
				i++;
				continue ;
			}
			b = (t_bunnies){tokens, quote_type, expand_env, i, NULL};
			bunnies_help(&b);
		}
		i++;
	}
}

void	handle_single_command(t_shell *shell, char *input)
{
	char	**tokens;

	tokens = ft_split(input, ' ');
	free(input);
	if (!tokens[0])
		return (free_tokens(tokens));
	if (ft_strchr(tokens[1], '"') || ft_strchr(tokens[1], '\''))
	{
		handle_bunnies(tokens, '\'', 0);
		handle_bunnies(tokens, '"', 1);
	}
	else
		are_we_rich(tokens);
	if (!handle_builtin(tokens, shell))
		handle_command(tokens, shell->envp);
	free_tokens(tokens);
}
