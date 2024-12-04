/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 16:59:17 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/06 17:01:57 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	result = (char *)malloc(len_s1 + len_s2 + 1);
	if (!result)
		return (NULL);
	if (len_s1 > 0)
		ft_strlcpy(result, s1, len_s1 + 1);
	if (len_s2 > 0)
		ft_strlcpy(result + len_s1, s2, len_s2 + 1);
	return (result);
}

/*int main ()
{
	char s1[] = "hello";
	char s2[] = "world";
	printf("%s", ft_strjoin(s1, s2));
}*/
