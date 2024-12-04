/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 17:58:19 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/01 18:14:55 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	total_len;
	size_t	i;

	i = 0;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	total_len = dst_len + src_len;
	if (size <= dst_len)
		return (src_len + size);
	if (size > 0)
	{
		while ((i < size - dst_len - 1) && src[i] != '\0')
		{
			dst[dst_len + i] = src[i];
			i++;
		}
	}
	dst[dst_len + i] = '\0';
	return (total_len);
}

/*int	main(void)
{
	char dst[20] = "Hello";
	const char *src = " world!";
	size_t size = sizeof(dst);

	printf("Before ft_strlcat:\n");
	printf("dst: %s\n", dst);
	printf("src: %s\n", src);

	size_t result = ft_strlcat(dst, src, size);

	printf("\nAfter ft_strlcat:\n");
	printf("dst: %s\n", dst);
	printf("src: %s\n", src);
	printf("Result: %zu\n", result);

	return (0);
}*/
