/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:07 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/01/20 19:56:26 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_exit_status = 0;

static void	handle_pipes(char **commands, char **envp, char **tokens, int i)
{
	int		fddebug;
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	char	*path;

	prev_fd = 0;
	while (commands[i])
	{
		if (commands[i + 1] && pipe(fd) == -1)
		{
			write(2, "pipe error\n", 11);
			break ;
		}
		pid = fork();
		if (pid == -1)
		{
			write(2, "fork error\n", 11);
			break ;
		}
		if (pid == 0)
		{
			if (prev_fd != 0)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (commands[i + 1])
				dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			tokens = ft_split(commands[i], ' ');
			if (handle_redirections(tokens) == -1)
			{
				write(2, "redirection error\n", 18);
				free_tokens(tokens);
				exit(EXIT_FAILURE);
			}
			handle_quotes(tokens);
			path = get_path(tokens[0]);
			if (path)
			{
				fddebug = open("debug", O_RDWR | O_APPEND, 0777);
				write(fddebug, tokens[1], 20);
				close(fddebug);
				execve(path, tokens, envp);
				perror("execve");
			}
			else
			{
				write(2, "Command not found: ", 19);
				write(2, tokens[0], ft_strlen(tokens[0]));
				write(2, "\n", 1);
			}
			free_tokens(tokens);
			exit(127);
		}
		else
		{
			waitpid(pid, &g_exit_status, 0);
			if (WIFEXITED(g_exit_status))
			{
				g_exit_status = WEXITSTATUS(g_exit_status);
			}
			close(fd[1]);
			if (prev_fd != 0)
				close(prev_fd);
			prev_fd = fd[0];
		}
		i++;
	}
	if (prev_fd != 0)
		close(prev_fd);
}

static int	handle_builtin(char **tokens, t_shell *shell)
{
	int		is_num;
	char	*arg;

	if (ft_strncmp(tokens[0], "exit", 4) == 0 && ft_strlen(tokens[0]) == 4)
	{
		is_num = 1;
		if (tokens[1])
		{
			arg = tokens[1];
			while (*arg)
			{
				if (ft_isdigit(*arg) == 0)
				{
					is_num = 0;
					break ;
				}
				arg++;
			}
		}
		if (!tokens[1] || is_num)
		{
			write(1, "\033[38;2;255;105;180mBye Bitch ;*\033[0m\n", 37);
			free_tokens(tokens);
			free_custom_env(shell->custom_env);
			clear_history();
			exit (g_exit_status);
		}
		else
			write(1, "Invalid exit argument\n", 22);
	}
	else if (ft_strncmp(tokens[0], "cd", 2) == 0 && ft_strlen(tokens[0]) == 2)
	{
		builtin_cd(tokens);
		g_exit_status = 0;
	}
	else if (ft_strncmp(tokens[0], "unset", 5) == 0
		&& ft_strlen(tokens[0]) == 5)
	{
		builtin_unset(shell, tokens);
		g_exit_status = 0;
	}
	else if (ft_strncmp(tokens[0], "export", 6) == 0
		&& ft_strlen(tokens[0]) == 6)
	{
		builtin_export(shell, tokens, 1, 0);
		g_exit_status = 0;
	}
	else if (ft_strncmp(tokens[0], "env", 3) == 0 && ft_strlen(tokens[0]) == 3)
	{
		builtin_env(shell);
		g_exit_status = 0;
	}
	else
		return (0);
	return (1);
}

static void	handle_command(char **tokens, char **envp)
{
	pid_t	pid;
	char	*path;
	int		fddebug;

	pid = fork();
	if (pid == -1)
	{
		write(2, "fork error\n", 11);
		return ;
	}
	if (pid == 0)
	{
		if (handle_redirections(tokens) == -1)
		{
			write(2, "redirection error\n", 18);
			free_tokens(tokens);
			exit(EXIT_FAILURE);
		}
		if (ft_strchr(tokens[0], '/') && access(tokens[0], X_OK) == 0)
			path = tokens[0];
		else
			path = get_path(tokens[0]);
		if (path)
		{
			fddebug = open("debug", O_RDWR | O_APPEND, 0777);
			write(fddebug, tokens[1], 8);
			close(fddebug);
			execve(path, tokens, envp);
			perror("execve");
		}
		else
		{
			write(2, "Command not found: ", 19);
			write(2, tokens[0], ft_strlen(tokens[0]));
			write(2, "\n", 1);
		}
		free_tokens(tokens);
		exit(127);
	}
	waitpid(pid, &g_exit_status, 0);
	if (WIFEXITED(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**tokens;
	char	**commands;
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.envp = envp;
	shell.custom_env = NULL;
	setup_signal_handlers();
	shell.custom_env = init_env(envp);
	while (1)
	{
		input = readline(
				"\001\033[38;2;255;105;180m\002Barbie Bash 💅\001\033[0m\002: ");
		if (!input)
		{
			write(1, "\n\033[38;2;255;105;180mBye Bitch ;*\033[0m\n", 37);
			break ;
		}
		if (*input)
			add_history(input);
		if (ft_strchr(input, '|'))
		{
			commands = ft_split(input, '|');
			handle_pipes(commands, shell.envp, tokens, 0);
			free_tokens(commands);
		}
		else
		{
			tokens = ft_split(input, ' ');
			free(input);
			if (!tokens[0])
			{
				free_tokens(tokens);
				continue ;
			}
			if (ft_strchr(tokens[1], '"') || ft_strchr(tokens[1], '\''))
			{
				handle_bunnies(tokens, '\'', 0);
				handle_bunnies(tokens, '"', 1);
			}
			else
				are_we_rich(tokens);
			if (handle_builtin(tokens, &shell))
			{
				free_tokens(tokens);
				continue ;
			}
			else
				handle_command(tokens, shell.envp);
			free_tokens(tokens);
		}
	}
	free_custom_env(shell.custom_env);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	return (0);
}
