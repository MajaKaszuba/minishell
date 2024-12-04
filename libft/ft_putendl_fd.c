/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 16:12:21 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/08 16:15:41 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <string.h>
#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	write(fd, s, ft_strlen((const char *)s));
	write(fd, "\n", 1);
}

/*int	main()
{
	char s[] = "Hello!";
	int fd = 1;
	ft_putendl_fd(s, fd);
}*/
