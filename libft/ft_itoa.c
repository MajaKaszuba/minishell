/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 14:05:48 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/10 14:09:19 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include "libft.h"
#include <stdlib.h>

static size_t	ft_len(int n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

static int	ft_sign(int n)
{
	int	sign;

	sign = 1;
	if (n < 0)
		sign = -1;
	return (sign);
}

char	*ft_build(size_t len, char *result, int n)
{
	while (n != 0)
	{
		len--;
		result[len] = ft_abs(n % 10) + '0';
		n /= 10;
	}
	return (result);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		sign;
	size_t	len;

	len = ft_len(n);
	sign = ft_sign(n);
	if (n < 0)
		len++;
	result = malloc((len + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	result[len] = '\0';
	if (n == 0)
		result[0] = '0';
	else
	{
		if (sign == -1)
			result[0] = '-';
		ft_build(len, result, n);
	}
	return (result);
}

/*int main(void)
{
    int num1 = 12345;
    int num2 = -9876;
    int num3 = 0;

    char *str1 = ft_itoa(num1);
    char *str2 = ft_itoa(num2);
    char *str3 = ft_itoa(num3);

    printf("Integer: %d, String: %s\n", num1, str1);
    printf("Integer: %d, String: %s\n", num2, str2);
    printf("Integer: %d, String: %s\n", num3, str3);

    // Don't forget to free the allocated memory
    free(str1);
    free(str2);
    free(str3);

    return 0;
}*/
