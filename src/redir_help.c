/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:09:20 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/07 17:56:50 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_append_redirection(char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		write(STDERR_FILENO,
			"syntax error: expected filename after '>>'\n", 42);
		return (-1);
	}
	if (redirect_output_append(tokens[i + 1]) == -1)
		return (-1);
	tokens[i] = NULL;
	tokens[i + 1] = NULL;
	return (1);
}

static int	handle_heredoc_redirection(char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		write(STDERR_FILENO,
			"syntax error: expected delimiter after '<<'\n", 43);
		return (-1);
	}
	better_readline(tokens[i + 1]);
	tokens[i] = NULL;
	tokens[i + 1] = NULL;
	return (1);
}

static int	handle_output_redirection(char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		write(STDERR_FILENO,
			"syntax error: expected filename after '>'\n", 42);
		return (-1);
	}
	if (redirect_output(tokens[i + 1]) == -1)
		return (-1);
	tokens[i] = NULL;
	tokens[i + 1] = NULL;
	return (1);
}

static int	handle_input_redirection(char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		write(STDERR_FILENO,
			"syntax error: expected filename after '<'\n", 42);
		return (-1);
	}
	if (redirect_input(tokens[i + 1]) == -1)
		return (-1);
	tokens[i] = NULL;
	tokens[i + 1] = NULL;
	return (1);
}

int	handle_redirections(char **tokens)
{
	int	i;
	int	ret;

	i = 0;
	while (tokens[i])
	{
		if (ft_strncmp(tokens[i], ">>", 2) == 0)
			ret = handle_append_redirection(tokens, i);
		else if (ft_strncmp(tokens[i], "<<", 2) == 0)
			ret = handle_heredoc_redirection(tokens, i);
		else if (ft_strchr(tokens[i], '>'))
			ret = handle_output_redirection(tokens, i);
		else if (ft_strchr(tokens[i], '<'))
			ret = handle_input_redirection(tokens, i);
		else
		{
			i++;
			continue ;
		}
		if (ret == -1)
			return (-1);
		i++;
	}
	return (0);
}
