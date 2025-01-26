#include "../include/minishell.h"

int	find_closing_quote(char **tokens, int start, char quote_type)
{
	int	i;

	i = start;
	while (tokens[i])
	{
		if ((i == start && tokens[i][ft_strlen(tokens[i]) - 1] == quote_type)
			|| (i != start && tokens[i][ft_strlen
			(tokens[i]) - 1] == quote_type))
			return (i);
		i++;
	}
	return (-1);
}

char	*merge_tokens(char **tokens, int start, int end, char quote_type)
{
	char	*merged;
	int		i;

	merged = ft_strdup(tokens[start] + 1);
	i = start + 1;
	while (i <= end)
	{
		merged = ft_strjoin(merged, " ");
		merged = ft_strjoin(merged, tokens[i]);
		i++;
	}
	if (merged[ft_strlen(merged) - 1] == quote_type)
		merged[ft_strlen(merged) - 1] = '\0';
	return (merged);
}

void	remove_merged_tokens(char **tokens, int start, int end)
{
	int	i;

	i = start + 1;
	while (tokens[end + 1])
	{
		tokens[i] = tokens[end + 1];
		i++;
		end++;
	}
	while (tokens[i])
	{
		tokens[i] = NULL;
		i++;
	}
}

void	handle_bunnies(char **tokens, char quote_type, int expand_env)
{
	int		i;
	int		closing_quote;
	char	*merged;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == quote_type)
		{
			if (tokens[i][ft_strlen(tokens[i]) - 1] == quote_type)
			{
				merged = ft_strdup(tokens[i] + 1);
				merged[ft_strlen(merged) - 1] = '\0';
				free(tokens[i]);
				tokens[i] = merged;
				if (expand_env && quote_type == '"')
					are_we_rich(tokens + i);
				i++;
				continue ;
			}
			closing_quote = find_closing_quote(tokens, i, quote_type);
			if (closing_quote == -1)
			{
				shell_error(ft_strjoin
					("unmatched ", ft_strjoin_char("", quote_type)), 2);
				return ;
			}
			merged = merge_tokens(tokens, i, closing_quote, quote_type);
			free(tokens[i]);
			tokens[i] = merged;
			if (expand_env && quote_type == '"')
				are_we_rich(tokens + i);
			remove_merged_tokens(tokens, i, closing_quote);
		}
		else
			i++;
	}
}
