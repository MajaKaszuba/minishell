/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olaf <olaf@student.1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:18:35 by olaf              #+#    #+#             */
/*   Updated: 2025/01/12 18:41:16 by olaf             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    sigint_handler(int signo)
{
    (void)signo;
    rl_replace_line("", 0);
    rl_on_new_line();
    write(STDOUT_FILENO, "\n", 1);
    rl_redisplay();
}

void    setup_signal_handlers(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}