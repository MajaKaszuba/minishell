/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:43:19 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/08 14:45:08 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	count_words(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			count++;
			in_word = 1;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static int	word_length(char const *s, char c)
{
	int	length;

	length = 0;
	while (*s && *s != c)
	{
		length++;
		s++;
	}
	return (length);
}

char	**ft_split(char const *s, char c)
{
	int		num_words;
	char	**result;
	int		i;

	if (!s)
		return (NULL);
	num_words = count_words(s, c);
	result = malloc((num_words + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (i < num_words)
	{
		while (*s == c)
			s++;
		result[i] = ft_substr(s, 0, word_length(s, c));
		if (!result[i])
			return (NULL);
		s += word_length(s, c);
		i++;
	}
	result[i] = NULL;
	return (result);
}

// int main() {
//     const char* s = "lorem ipsum dolor sit amet.";
//     char c = ' ';
//     char** result = ft_split(s, c);
//     for (int i = 0; result[i] != NULL; i++) {
//         printf("%s\n", result[i]);
//     }
// 		free(result);
//     return 0;
// }
