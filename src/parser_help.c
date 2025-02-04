/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:09:42 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/04 23:47:02 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	bunnies_help(t_bunnies *b)
{
	int	closing_quote;

	closing_quote = find_closing_quote(b->tokens, b->index, b->quote_type);
	if (closing_quote == -1)
	{
		shell_error(ft_strjoin
			("unmatched ", ft_strjoin_char("", b->quote_type)), 2);
		return ;
	}
	b->merged = merge_tokens(b->tokens, b->index, closing_quote, b->quote_type);
	free(b->tokens[b->index]);
	b->tokens[b->index] = b->merged;
	if (b->expand_env && b->quote_type == '"')
		are_we_rich(b->tokens + b->index);
	remove_merged_tokens(b->tokens, b->index, closing_quote);
}

void	handle_simple_quotes(
	char **tokens, char quote_type, int expand_env, int i)
{
	char	*merged;

	merged = ft_strdup(tokens[i] + 1);
	merged[ft_strlen(merged) - 1] = '\0';
	free(tokens[i]);
	tokens[i] = merged;
	if (expand_env && quote_type == '"')
		are_we_rich(tokens + i);
}
