/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olaf <olaf@student.1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:09:39 by olaf              #+#    #+#             */
/*   Updated: 2024/12/16 18:31:37 by olaf             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	redirect_output(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
		//error opening file
		return (-1);
    }
    if (dup2(fd, STDOUT_FILENO) < 0) //przekierowanie stdout na plik
    {
    	//error duplicating output
        close(fd);
        return (-1);
    }
    close(fd);
    return(0);
}

int	redirect_input(char *filename)
{
	int	fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
    	//error opening file
        return (-1);
    }
    if (dup2(fd, STDIN_FILENO) < 0) //przekierowanie stdin na plik
    {
    	//error duplicating input
        close (fd);
        return (-1);
    }
    close(fd);
    return(0);
}

int handle_redirections(char **tokens)
{
	int i = 0;

	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '>')) // Przekierowanie wyjścia ">"
		{
			if (!tokens[i + 1]) // Brak nazwy pliku
			{
				write(STDERR_FILENO, "syntax error: expected filename after '>'\n", 42);
				return (-1);
			}
			if (redirect_output(tokens[i + 1]) == -1)
				return (-1);
			tokens[i] = NULL;
			tokens[i + 1] = NULL;
			i++;
		}
		else if (ft_strchr(tokens[i], '<')) // Przekierowanie wejścia "<"
		{
			if (!tokens[i + 1]) // Brak nazwy pliku
			{
				write(STDERR_FILENO, "syntax error: expected filename after '<'\n", 42);
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