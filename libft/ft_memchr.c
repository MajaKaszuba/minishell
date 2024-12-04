/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:17:56 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/02 13:23:37 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	char	*p;

	p = (char *)s;
	while (n > 0)
	{
		if (*p == (char)c)
			return (p);
		p++;
		n--;
	}
	return (NULL);
}

/*int main()
{
	unsigned char data[] = {10, 20, 30, 40, 50, 60, 70, 80};
	int search_value = 50;
	size_t data_size = sizeof(data);
	void *result = ft_memchr(data, search_value, data_size);
	printf("%d", search_value);
}*/
