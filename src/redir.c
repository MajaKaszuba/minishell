/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:09:39 by olaf              #+#    #+#             */
/*   Updated: 2025/01/13 16:19:23 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	redirect_output(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirect_output_append(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open append");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 append");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirect_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close (fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirect_input_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(STDERR_FILENO, "warning: heredoc delimited by EOF\n", 34);
			break ;
		}
		if (ft_strlen(line) == ft_strlen(delimiter)
			&& ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(pipe_fd[0]);
		return (-1);
	}
	close(pipe_fd[0]);
	return (0);
}

int	handle_redirections(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (ft_strncmp(tokens[i], ">>", 2) == 0)
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
			i++;
		}
		else if (ft_strncmp(tokens[i], "<<", 2) == 0)
		{
			if (!tokens[i + 1])
			{
				write(STDERR_FILENO,
					"syntax error: expected delimiter after '<<'\n", 43);
				return (-1);
			}
			if (redirect_input_heredoc(tokens[i + 1]) == -1)
				return (-1);
			tokens[i] = NULL;
			tokens[i + 1] = NULL;
			i++;
		}
		else if (ft_strchr(tokens[i], '>'))
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
			i++;
		}
		else if (ft_strchr(tokens[i], '<'))
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
			i++;
		}
		i++;
	}
	return (0);
}
