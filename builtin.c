/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:15 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/11/27 15:50:12 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	builtin_cd(char **tokens)
{
	if (tokens[1])
	{
		if (chdir(tokens[1]) == -1)
			perror("cd");
	}
	else
	{
		char *home = getenv("HOME");
		if (home && chdir(home) == -1)
			perror("cd");
	}
}
