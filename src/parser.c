/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:26 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/01/13 16:16:02 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_custom_env(char **custom_env)
{
	int	i;

	i = 0;
	while (custom_env[i])
	{
		free(custom_env[i]);
		i++;
	}
	free(custom_env);
}

char	**init_env(char **envp)
{
	int		i;
	char	**new_env;

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	*get_env_value(char *token, int start, int end)
{
	char	*var_name;
	char	*env_value;

	var_name = ft_substr(token, start, end - start + 1);
	if (!var_name)
		return (ft_strdup(""));
	if (ft_strncmp(var_name, "?", 2) == 0)
	{
		free(var_name);
		return (ft_itoa(g_exit_status));
	}
	env_value = getenv(var_name);
	free(var_name);
	if (!env_value)
		return (ft_strdup(""));
	return (ft_strdup(env_value));
}

char	*expand_env_variables(char *token)
{
	char	*temp;
	char	*result;
	char	*env_value;
	int		i;
	int		start;

	result = ft_strdup("");
	if (!result)
		return (ft_strdup(""));
	i = 0;
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] != '\0')
		{
			start = i + 1;
			if (token[start] == '?')
			{
				env_value = ft_itoa(g_exit_status);
				i++;
			}
			else
			{
				while (token[i + 1] && (ft_isalnum(token[i + 1])
						|| token[i + 1] == '_'))
					i++;
				env_value = get_env_value(token, start, i);
			}
			if (!env_value)
				env_value = ft_strdup("");
			temp = ft_strjoin(result, env_value);
			free(result);
			free(env_value);
			result = temp;
		}
		else
		{
			temp = ft_strjoin_char(result, token[i]);
			free(result);
			result = temp;
		}
		i++;
	}
	return (result);
}

void	are_we_rich(char **tokens)
{
	int		i;
	char	*expanded;

	i = 0;
	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '$'))
		{
			expanded = expand_env_variables(tokens[i]);
			free(tokens[i]);
			tokens[i] = expanded;
		}
		i++;
	}
}

int	find_closing_quote(char **tokens, int start, char quote_type)
{
	int	i;

	i = start;
	while (tokens[i])
	{
		if ((i == start && tokens[i][ft_strlen(tokens[i]) - 1] == quote_type)
			|| (i != start && tokens[i][ft_strlen
			(tokens[i]) - 1] == quote_type))
			return (i);
		i++;
	}
	return (-1);
}

char	*merge_tokens(char **tokens, int start, int end, char quote_type)
{
	char	*merged;
	int		i;

	merged = ft_strdup(tokens[start] + 1);
	i = start + 1;
	while (i <= end)
	{
		merged = ft_strjoin(merged, " ");
		merged = ft_strjoin(merged, tokens[i]);
		i++;
	}
	if (merged[ft_strlen(merged) - 1] == quote_type)
		merged[ft_strlen(merged) - 1] = '\0';
	return (merged);
}

void	remove_merged_tokens(char **tokens, int start, int end)
{
	int	i;

	i = start + 1;
	while (tokens[end + 1])
	{
		tokens[i] = tokens[end + 1];
		i++;
		end++;
	}
	while (tokens[i])
	{
		tokens[i] = NULL;
		i++;
	}
}

void	handle_bunnies(char **tokens, char quote_type, int expand_env)
{
	int		i;
	int		closing_quote;
	char	*merged;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == quote_type)
		{
			if (tokens[i][ft_strlen(tokens[i]) - 1] == quote_type)
			{
				merged = ft_strdup(tokens[i] + 1);
				merged[ft_strlen(merged) - 1] = '\0';
				free(tokens[i]);
				tokens[i] = merged;
				if (expand_env)
					are_we_rich(tokens + i);
				i++;
				continue ;
			}
			closing_quote = find_closing_quote(tokens, i, quote_type);
			if (closing_quote == -1)
			{
				shell_error(ft_strjoin
					("unmatched ", ft_strjoin_char("", quote_type)), 2);
				return ;
			}
			merged = merge_tokens(tokens, i, closing_quote, quote_type);
			free(tokens[i]);
			tokens[i] = merged;
			if (expand_env)
				are_we_rich(tokens + i);
			remove_merged_tokens(tokens, i, closing_quote);
		}
		else
			i++;
	}
}
