#include "../include/minishell.h"

char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = (char *)malloc(ft_strlen(str) + 1);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			j++;
		i++;
	}
	result = malloc(j + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	handle_quotes(char **tokens)
{
	int		i;
	char	*new_str;

	i = 0;
	while (tokens[i])
	{
		new_str = remove_quotes(tokens[i]);
		free(tokens[i]);
		tokens[i] = new_str;
		i++;
	}
}