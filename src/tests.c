/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olaf <olaf@student.1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:55:58 by olaf              #+#    #+#             */
/*   Updated: 2024/12/08 02:29:14 by olaf             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void  execution(char *command, char **tokens, char **paths)
{
  if (ft_strchr(command, '/'))
    {
    	if (access(tokens[1], X_OK) == -1)
        {
          shell_error(ft_strjoin("error: no access", command), 1);
          return ;
        }
        if (execve(command, tokens, paths) == -1)
        {
        	shell_error(ft_strjoin("error: failed to run executable", command), 1);
        	return ;
        }
    }
}