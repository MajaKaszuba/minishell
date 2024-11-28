/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:26 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/11/27 16:23:11 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	are_we_rich(char **tokens)
{
	int		i;
	char	*env_value;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '$')
		{
			env_value = getenv(tokens[i] + 1);
			if (env_value)
			{
				free(tokens[i]);
				tokens[i] = ft_strdup(env_value);
			}
		}
		i++;
	}
	return (0);
}

int	find_closing_quote(char **tokens, int start, char quote_type)
{
	int	i;

	i = start + 1;
	while (tokens[i])
	{
		if (tokens[i][ft_strlen(tokens[i]) - 1] == quote_type)
			return (i);
		i++;
	}
	return (-1); // Brak zamykającego cudzysłowu
}

char	*merge_tokens(char **tokens, int start, int end, char quote_type)
{
	char	*merged;
	char	*temp;
	int		i;

	merged = ft_strdup(tokens[start] + 1); // Pomijamy otwierający cudzysłów
	i = start + 1;
	while (i <= end)
	{
		temp = merged;
		merged = ft_strjoin(merged, " ");
		free(temp);
		temp = merged;
		merged = ft_strjoin(merged, tokens[i]);
		free(temp);
		i++;
	}
	if (merged[ft_strlen(merged) - 1] == quote_type)
		merged[ft_strlen(merged) - 1] = '\0'; // Usuwamy zamykający cudzysłów
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

void	single_bunny(char **tokens)
{
	int		i;
	int		closing_quote;
	char	*merged;
	char	quote_type;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '\'') // Znajdź otwierający cudzysłów
		{
			quote_type = tokens[i][0];
			closing_quote = find_closing_quote(tokens, i, quote_type);
			if (closing_quote == -1) // Brak zamykającego cudzysłowu
			{
				printf("Error: unmatched single quote in input.\n");
				return ;
			}
			merged = merge_tokens(tokens, i, closing_quote, quote_type);
			free(tokens[i]);
			tokens[i] = merged;
			remove_merged_tokens(tokens, i, closing_quote);
		}
		else
			i++;
	}
}
