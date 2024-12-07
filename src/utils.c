/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:29 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/12/07 18:02:00 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	validate_syntax(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		// Błędne znaki: ;;
		if (ft_strncmp(tokens[i], ";", 2) == 0) // Porównanie znaku `;`
		{
			if (!tokens[i + 1] || ft_strncmp(tokens[i + 1], ";", 2) == 0)
			{
				shell_error("syntax error near unexpected token `;'", 2);
				return (0);
			}
		}
		// Błędne znaki: ><
		if (ft_strncmp(tokens[i], ">", 2) == 0 && 
			tokens[i + 1] && ft_strncmp(tokens[i + 1], "<", 2) == 0)
		{
			shell_error("syntax error near unexpected token `><'", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

char	*ft_strjoin_char(char *s, char c)
{
	char	*new_str;
	int		len;

	len = ft_strlen(s);
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s, len + 1);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

void	shell_error(char *message, int exit_code)
{
	if (message)
	{
		write(2, "minishell: ", 11);
		write(2, message, ft_strlen(message));
		write(2, "\n", 1);
	}
	if (exit_code >= 0 && exit_code != 1) // Tylko w przypadku krytycznego błędu
		exit(exit_code);
}

int is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (!str || !(ft_isalpha(str[0]) || str[0] == '_')) // Musi zaczynać się od litery lub '_'
		return (0);
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_' || str[i] == '=')) // Reszta musi być alfanumeryczna lub '_'
			return (0);
		i++;
	}
	return (1); // Jeśli wszystkie warunki spełnione
}
