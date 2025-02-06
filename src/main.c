/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:07 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/07 00:15:18 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_exit_status = 0;

static void	handle_line(t_shell *shell, char *input)
{
	char	**commands;

	if (ft_strchr(input, '|'))
	{
		commands = ft_split(input, '|');
		handle_pipes(commands, shell->envp, NULL, 0);
		free_tokens(commands);
	}
	else
		handle_single_command(shell, input);
}

void	shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline
			("\001\033[38;2;255;105;180m\002Barbie Bash 💅\001\033[0m\002: ");
		if (!input)
		{
			write(STDOUT_FILENO,
				"\033[38;2;255;105;180mBye Bitch ;*\033[0m\n", 37);
			exit(g_exit_status);
		}
		if (*input)
			add_history(input);
		handle_line(shell, input);
	}
}

static void	cleanup_shell(t_shell *shell)
{
	if (shell->custom_env)
		free_custom_env(shell->custom_env);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
}

static void	init_shell(t_shell *shell, char **envp)
{
	shell->envp = envp;
	shell->custom_env = init_env(envp);
	setup_signal_handlers();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	shell_loop(&shell);
	cleanup_shell(&shell);
	return (0);
}
