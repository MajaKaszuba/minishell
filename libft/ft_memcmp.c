/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:48:51 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/02 13:56:12 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*p1;
	const unsigned char	*p2;
	size_t				i;

	p1 = s1;
	p2 = s2;
	i = 0;
	while (i < n)
	{
		if (p1[i] < p2[i])
		{
			return (-1);
		}
		else if (p1[i] > p2[i])
		{
			return (1);
		}
		i++;
	}
	return (0);
}

/*int main()
{
	unsigned char s1[] = {10, 20, 30, 40, 50};
	unsigned char s2[] = {10, 20, 30, 40, 60};
	size_t n = sizeof(s1);
	printf("%d", ft_memcmp(s1, s2, n));
}*/
