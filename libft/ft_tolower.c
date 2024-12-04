/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 18:52:44 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/01 18:55:15 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>

int	ft_tolower(int c)
{
	if (c > 64 && c < 91)
	{
		c += 32;
		return (c);
	}
	else
	{
		return (c);
	}
}

/*int main ()
{
	int c = 116;
	c = ft_tolower(c);
	printf("%c", c);
}*/
