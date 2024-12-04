/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 13:48:45 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/10 13:50:41 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	len;
	unsigned int	i;
	char			*result;

	if (s == NULL || f == NULL)
		return (NULL);
	len = ft_strlen((char *)s);
	result = (char *)malloc(len + 1);
	if (result == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = f(i, s[i]);
		i++;
	}
	result[len] = '\0';
	return (result);
}

/*int main() {
    const char s[] = "Hello, World!";
    char *resultString = ft_strmapi(s, &f);
    if (resultString != NULL) {
        printf("Original String: %s\n", s);
        printf("Modified String: %s\n", resultString);
    } else
        printf("Memory allocation failed.\n");
}*/
