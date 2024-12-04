/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 12:49:44 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/19 18:01:59 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] != (char)c)
		{
			i--;
		}
		else
		{
			return ((char *)(s + i));
		}
	}
	return (NULL);
}

/*int main ()
{
	char s[] = "helloohihihi";
	int c = 'h';
	printf("%s", ft_strrchr(s, c));
}*/
