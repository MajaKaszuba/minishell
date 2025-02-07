/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:09:39 by olaf              #+#    #+#             */
/*   Updated: 2025/02/07 17:42:36 by mkaszuba         ###   ########.fr       */
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

int	get_input(char **line)
{
	char	*buffer;
	char	c;
	int		bytes_read;
	int		i;

	i = 0;
	bytes_read = 0;
	buffer = (char *)malloc(1000);
	if (!buffer)
		return (-1);
	write(0, "> ", 2);
	bytes_read = read(0, &c, 1);
	while (bytes_read && c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		bytes_read = read(0, &c, 1);
	}
	buffer[i] = '\n';
	buffer[++i] = '\0';
	*line = ft_strdup(buffer);
	free(buffer);
	return (bytes_read);
}

void	better_readline(char *delimiter)
{
	char	*line;
	int		fd[2];
	pid_t	line_reader;

	if (pipe(fd) == -1)
		shell_error("", 1);
	line_reader = fork();
	if(line_reader == 0)
	{
		close(fd[0]);
		while(get_input(&line))
		{
			if(ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
				exit(0);
			write(fd[1], line, ft_strlen(line));
			free(line);
		}
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	wait(NULL);
}
