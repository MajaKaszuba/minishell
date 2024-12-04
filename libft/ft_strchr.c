/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 18:57:25 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/19 18:01:45 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	int		i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != (char)c)
		{
			i++;
		}
		else
		{
			return ((char *)(s + i));
		}
	}
	if (c == '\0')
		return ((char *)(s + i));
	return (NULL);
}

/*int main ()
{
	char s[] = "hello";
	int c = '\0';
	printf("%s", ft_strchr(s, c));
}*/
