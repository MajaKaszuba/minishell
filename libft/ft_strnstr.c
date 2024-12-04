/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 14:05:36 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/02 14:21:04 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>

static int	leng(const char *little, size_t lit_len)
{
	while (little[lit_len] != '\0')
	{
		lit_len++;
	}
	return (lit_len);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	lit_len;
	size_t	i;
	size_t	j;

	lit_len = 0;
	i = 0;
	j = 0;
	lit_len = leng(little, lit_len);
	if (lit_len == 0)
		return ((char *)big);
	while (i < len && big[i] != '\0')
	{
		if (big[i] == little[0])
		{
			while (j < lit_len && (i + j) < len && big[i + j] == little[j])
				j++;
			if (j == lit_len)
				return ((char *)(big + i));
		}
		i++;
	}
	return (NULL);
}

/*int main()
{
	const char big[] = "Hello world slay";
	const char little[] = "world";
	size_t len = 15;
	printf("%s\n", ft_strnstr(big, little, len));
}*/
