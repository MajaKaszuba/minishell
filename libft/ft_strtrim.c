/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:02:28 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/08 14:03:56 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>

static char	*s_start(const char *s1, const char *set)
{
	while (*s1 && ft_strchr(set, *s1))
		s1++;
	return ((char *)s1);
}

static char	*s_end(char *s1, const char *set)
{
	char	*end;

	end = s1 + ft_strlen(s1) - 1;
	while (end > s1 && ft_strchr(set, *end))
		end--;
	return (end);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmed;
	char	*start;
	char	*end;
	size_t	trimmedlen;

	if (s1 == NULL)
		return (NULL);
	start = s_start(s1, set);
	if (*start == '\0')
	{
		trimmed = (char *)malloc(1);
		if (trimmed == NULL)
			return (NULL);
		trimmed[0] = '\0';
		return (trimmed);
	}
	end = s_end(start, set);
	trimmedlen = (size_t)(end - start + 1);
	trimmed = (char *)malloc(trimmedlen + 1);
	if (trimmed == NULL)
		return (NULL);
	ft_strlcpy(trimmed, start, trimmedlen + 1);
	trimmed[trimmedlen] = '\0';
	return (trimmed);
}

/*int main()
{
	const char s1[] = "  Hello, world ! ";
	const char set[] = "! ";
	printf("%s", ft_strtrim(s1, set));
}*/
