/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:26 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/04 23:46:42 by mkaszuba         ###   ########.fr       */
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

char	*help_expand(char *temp, char *token, char *result, int i)
{
	int		start;
	char	*env_value;

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
	return (result);
}

char	*expand_env_variables(char *token)
{
	char	*temp;
	char	*result;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (ft_strdup(""));
	i = 0;
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] != '\0')
			result = help_expand(temp, token, result, i);
		else
		{
			temp = ft_strjoin_char(result, 0);
			free(result);
			result = temp;
		}
		i++;
	}
	return (result);
}
