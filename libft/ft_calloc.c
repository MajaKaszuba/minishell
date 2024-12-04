/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:12:20 by mkaszuba          #+#    #+#             */
/*   Updated: 2024/03/05 19:22:16 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include <string.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*memory;

	if (size && (4294967295 / size < nmemb))
		return (NULL);
	memory = malloc(nmemb * size);
	if (memory != NULL)
		ft_memset(memory, 0, nmemb * size);
	return (memory);
}

/*int	main()
{	
	size_t	nmemb = 5;
	size_t size = sizeof(int);
	int* dyn_array = (int*)ft_calloc(nmemb, size);

	if (dyn_array != NULL) {
		for (size_t i = 0; i < nmemb; i++) {
			printf("dyn_array[%zu] = %d\n", i, dyn_array[i]);
		}

		free(dyn_array);
	} else {
		printf("Błąd alokacji pamięci.\n");
	}

	return 0;
}*/
