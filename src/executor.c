/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:19 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/12 17:17:43 by olaf             ###   ########.fr       */
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
	paths = ft_split(path_env, ':'); //Dzieli path na katalogi
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, command); //Tworzy pełną ścieżkę
		if (access(full_path, X_OK) == 0) //Sprawdza,czy plik istnieje i jest wykonywalny
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

void	execution(char *command, char **tokens, char **envp)
{
	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) != 0)
			shell_error(ft_strjoin("error: no access to ", command), 126);
		execve(command, tokens, envp);
		shell_error(ft_strjoin("error: failed to run executable ", command), 1);
		exit(errno);
	}
	else
	{
		char *path = get_path(command);
		if (!path)
		{
			shell_error(ft_strjoin("error: command not found: ", command), 1);
			exit(127);
		}
		execve(path, tokens, envp);
		shell_error(ft_strjoin("error: failed to run executable ", path), 1);
		free(path);
		exit(errno);
	}
}