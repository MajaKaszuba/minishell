/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:19 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/01/12 18:50:52 by olaf             ###   ########.fr       */
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
		full_path = ft_strjoin(paths[i], "/");//tu bedzie memory leak
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

