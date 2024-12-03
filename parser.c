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

char	*expand_env_variables(char *token)
{
	char	*result;
	char	*env_value;
	int		i;
	int		start;

	result = ft_strdup(""); // Pusty wynik
	if (!result)
		return (NULL);
	i = 0;
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] != '\0') // Znaleziono '$'
		{
			start = i + 1;
			while (token[i + 1] && (ft_isalnum(token[i + 1]) || token[i + 1] == '_'))
				i++;
			env_value = get_env_value(token, start, i);
			if (!env_value) // Obsługa błędów
			{
				free(result);
				return (NULL);
			}
			result = append_env_value(result, env_value);
			free(env_value);
		}
		else
			result = append_char(result, token[i]); // Dodaj bieżący znak
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
		if (ft_strchr(tokens[i], '$')) // Sprawdź, czy token zawiera '$'
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
		// Jeśli cudzysłów zamykający jest w tym samym tokenie
		if (i == start && tokens[i][ft_strlen(tokens[i]) - 1] == quote_type)
			return (i);
		// Jeśli cudzysłów zamykający jest w innym tokenie
		else if (i != start && tokens[i][ft_strlen(tokens[i]) - 1] == quote_type)
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

	i = 0;
	while (tokens[i])
	{
		// Znajdź otwierający pojedynczy cudzysłów
		if (tokens[i][0] == '\'')
		{
			closing_quote = find_closing_quote(tokens, i, '\'');
			if (closing_quote == -1) // Brak zamykającego cudzysłowu
			{
				printf("Error: unmatched single quote in input.\n");
				return ;
			}
			// Połącz tokeny w jeden ciąg
			merged = merge_tokens(tokens, i, closing_quote, '\'');
			free(tokens[i]);
			tokens[i] = merged;
			// Usuń przetworzone tokeny
			remove_merged_tokens(tokens, i, closing_quote);
		}
		else
			i++;
	}
}

void	double_bunny(char **tokens)
{
	int		i;
	int		closing_quote;
	char	*merged;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '"') // Znalezienie otwierającego cudzysłowu
		{
			// Sprawdź, czy zamykający cudzysłów jest w tym samym tokenie
			if (tokens[i][ft_strlen(tokens[i]) - 1] == '"')
			{
				merged = ft_strdup(tokens[i] + 1);
				merged[ft_strlen(merged) - 1] = '\0';
				free(tokens[i]);
				tokens[i] = merged;
				are_we_rich(tokens + i); // Zamień zmienne środowiskowe
				i++;
				continue;
			}
			// Szukanie zamykającego cudzysłowu w innych tokenach
			closing_quote = find_closing_quote(tokens, i, '"');
			if (closing_quote == -1) // Błąd: brak zamykającego cudzysłowu
			{
				printf("Error: unmatched double quote in input.\n");
				return ;
			}
			// Łączenie tokenów w jeden ciąg
			merged = merge_tokens(tokens, i, closing_quote, '"');
			free(tokens[i]);
			tokens[i] = merged;
			// Zamień zmienne środowiskowe w wynikowym ciągu
			are_we_rich(tokens + i);
			// Usunięcie połączonych tokenów
			remove_merged_tokens(tokens, i, closing_quote);
		}
		else
			i++;
	}
}
