/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:26 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/07 22:46:42 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env_value(char *token, int start, int end)
{
	char	*var_name;
	char	*env_value;
	char	*result;

	var_name = ft_substr(token, start, end - start + 1);
	if (!var_name)
	{
		free(var_name);
		return (NULL);
	}
	env_value = getenv(var_name);
	if (env_value)
		result = ft_strdup(env_value);
	else
		result = ft_strdup(""); // Jeśli zmienna nie istnieje, zwracamy pusty ciąg
	free(var_name);
	return (result);
}

// Rozszerza zmienne środowiskowe w tokenie
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
		if (token[i] == '$' && token[i + 1] != '\0')
		{
			start = i + 1;
			while (token[i + 1] && (ft_isalnum(token[i + 1]) || token[i + 1] == '_'))
				i++;
			env_value = get_env_value(token, start, i); // Pobierz wartość zmiennej
			if (!env_value)
				return (NULL); // Obsługa błędów pamięci
			result = ft_strjoin(result, env_value); // Połącz z wynikiem
			free(env_value);
		}
		else
			result = ft_strjoin_char(result, token[i]); // Dodaj bieżący znak
		i++;
	}
	return (result);
}

// Szuka $
void	are_we_rich(char **tokens)
{
	int		i;
	char	*expanded;

	i = 0;
	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '$'))
		{
			expanded = expand_env_variables(tokens[i]); // Rozszerza zmienne
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
		if ((i == start && tokens[i][ft_strlen(tokens[i]) - 1] == quote_type) || 
			(i != start && tokens[i][ft_strlen(tokens[i]) - 1] == quote_type))
			return (i);
		i++;
	}
	return (-1); // Brak zamykającego cudzysłowu
}

// Łączy tokeny, usuwając cudzysłowy i łącząc w jeden ciąg
char	*merge_tokens(char **tokens, int start, int end, char quote_type)
{
	char	*merged;
	int		i;

	merged = ft_strdup(tokens[start] + 1); // Pomijamy otwierający cudzysłów
	i = start + 1;
	while (i <= end)
	{
		merged = ft_strjoin(merged, " ");
		merged = ft_strjoin(merged, tokens[i]);
		i++;
	}
	if (merged[ft_strlen(merged) - 1] == quote_type)
		merged[ft_strlen(merged) - 1] = '\0'; // Usuwamy zamykający cudzysłów
	return (merged);
}

// Usuwa tokeny połączone w jeden
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
		if (tokens[i][0] == quote_type) // Znalezienie otwierającego cudzysłowu
		{
			// Sprawdza, czy zamykający cudzysłów jest w tym samym tokenie
			if (tokens[i][ft_strlen(tokens[i]) - 1] == quote_type)
			{
				merged = ft_strdup(tokens[i] + 1);
				merged[ft_strlen(merged) - 1] = '\0';
				free(tokens[i]);
				tokens[i] = merged;
				if (expand_env)
					are_we_rich(tokens + i);
				i++;
				continue;
			}
			// Szukanie zamykającego cudzysłowu w innych tokenach
			closing_quote = find_closing_quote(tokens, i, quote_type);
			if (closing_quote == -1)
			{
				shell_error(ft_strjoin("unmatched ", ft_strjoin_char("", quote_type)), 2);
				return ;
			}
			// Łączenie tokenów w jeden ciąg
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
