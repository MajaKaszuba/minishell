/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:22:30 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/01 15:46:49 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	char		*s;
	char		temp[50];
	size_t		i;

	i = 0;
	d = dest;
	s = (char *)src;
	while (i < n)
	{
		temp[i] = s[i];
		i++;
	}
	i = 0;
	while (i < n)
	{
		d[i] = temp[i];
		i++;
	}
	return (dest);
}
/*
int main()
{
	char dest[] = "abcdef";
	char src[] = "123456789";
	int n = 4;
	ft_memmove(dest, src, n);
	printf("%s", dest);
}*/
