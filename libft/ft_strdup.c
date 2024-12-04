/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:48:16 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/05 19:50:20 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*new_s;

	if (s == NULL)
		return (NULL);
	len = ft_strlen(s) + 1;
	new_s = malloc(len);
	if (new_s == NULL)
		return (NULL);
	ft_strlcpy(new_s, s, len);
	return (new_s);
}

/*int main() {
    const char* original_str = "Hello, World!";
    char* duplicated_str = ft_strdup(original_str);
    if (duplicated_str != NULL) {
        printf("Original string: %s\n", original_str);
        printf("Duplicated string: %s\n", duplicated_str);
        free(duplicated_str);
    } else {
        printf("Memory allocation failed.\n");
    }
    return 0;
}*/
