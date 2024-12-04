/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 16:32:56 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/06 16:37:07 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;
	size_t	copy;

	i = 0;
	if (s == NULL)
		return (NULL);
	while (s[i] != '\0')
		i++;
	if (start >= i)
		return (ft_calloc(1, 1));
	if (len > i - start)
		len = i - start;
	substr = (char *)malloc(len + 1);
	if (substr == NULL)
		return (NULL);
	copy = 0;
	while (copy < len)
	{
		substr[copy] = s[start + copy];
		copy++;
	}
	substr[len] = '\0';
	return (substr);
}

/*int main ()
{
	char s[] = "mamdosyc";
	size_t start = 0;
	size_t len = 4;
	printf("%s", ft_substr(s, start, len));
}*/
