/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:09:25 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/02 22:09:26 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	find_quote(char *str, int i, int j)
{
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			j++;
		i++;
	}
}

char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = (char *)malloc(ft_strlen(str) + 1);
	find_quote(str, i, j);
	result = malloc(j + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	handle_quotes(char **tokens)
{
	int		i;
	char	*new_str;

	i = 0;
	while (tokens[i])
	{
		new_str = remove_quotes(tokens[i]);
		free(tokens[i]);
		tokens[i] = new_str;
		i++;
	}
}
