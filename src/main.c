/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:43:07 by mkaszuba          #+#    #+#             */
/*   Updated: 2025/02/04 22:26:52 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_exit_status = 0;

static void	cmd_not_found(char **tokens)
{
	write(2, "Command not found: ", 19);
	write(2, tokens[0], ft_strlen(tokens[0]));
	write(2, "\n", 1);
	free_tokens(tokens);
	exit(127);
}

static void	execute_command(char *cmd, char **envp)
{
	char	**tokens;
	char	*path;

	tokens = ft_split(cmd, ' ');
	if (handle_redirections(tokens) == -1)
	{
		write(2, "redirection error\n", 18);
		free_tokens(tokens);
		exit(EXIT_FAILURE);
	}
	handle_quotes(tokens);
	path = get_path(tokens[0]);
	if (!path)
		cmd_not_found(tokens);
	execve(path, tokens, envp);
	perror("execve");
	free_tokens(tokens);
	exit(127);
}

static void	handle_child(char *cmd, char **envp, int prev_fd, int *fd)
{
	if (prev_fd != 0)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (fd[1] != -1)
		dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	execute_command(cmd, envp);
}

static int	fork_and_pipe(char **commands, char **envp, int *prev_fd, int i)
{
	int		fd[2];
	pid_t	pid;

	if (commands[i + 1] && pipe(fd) == -1)
		return (write(2, "pipe error\n", 11), -1);
	pid = fork();
	if (pid < 0)
		return (write(2, "fork error\n", 11), -1);
	if (pid == 0)
		handle_child(commands[i], envp, *prev_fd,
			fd);
	else
	{
		waitpid(pid, &g_exit_status, 0);
		if (WIFEXITED(g_exit_status))
			g_exit_status = WEXITSTATUS(g_exit_status);
		close(fd[1]);
		if (*prev_fd != 0)
			close(*prev_fd);
		*prev_fd = fd[0];
	}
	return (0);
}

void	handle_pipes(char **commands, char **envp, char **tokens, int i)
{
	int		prev_fd;

	(void)tokens;
	(void)i;
	prev_fd = 0;
	i = 0;
	while (commands[i])
	{
		if (fork_and_pipe(commands, envp, &prev_fd, i) == -1)
			break ;
		i++;
	}
	if (prev_fd != 0)
		close(prev_fd);
}

static void	builtin_help2(char **tokens, t_shell *shell)
{
	write(1, "\033[38;2;255;105;180mBye Bitch ;*\033[0m\n", 37);
	free_tokens(tokens);
	free_custom_env(shell->custom_env);
	clear_history();
	exit (g_exit_status);
}

static void	builtin_help(char **tokens, t_shell *shell)
{
	int		is_num;
	char	*arg;

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
		builtin_help2(tokens, shell);
	else
		write(1, "Invalid exit argument\n", 22);
}

static int	handle_builtin(char **tokens, t_shell *shell)
{
	if (ft_strncmp(tokens[0], "exit", 4) == 0 && ft_strlen(tokens[0]) == 4)
		builtin_help(tokens, shell);
	else if (ft_strncmp(tokens[0], "cd", 2) == 0 && ft_strlen(tokens[0]) == 2)
		builtin_cd(tokens);
	else if (ft_strncmp(tokens[0], "unset", 5) == 0
		&& ft_strlen(tokens[0]) == 5)
		builtin_unset(shell, tokens);
	else if (ft_strncmp(tokens[0], "export", 6) == 0
		&& ft_strlen(tokens[0]) == 6)
		builtin_export(shell, tokens, 1, 0);
	else if (ft_strncmp(tokens[0], "env", 3) == 0 && ft_strlen(tokens[0]) == 3)
		builtin_env(shell);
	else
		return (0);
	return (1);
}

static void	command_help2(char **tokens, char **envp)
{
	char	*path;
	int		fddebug;

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
}

static void	command_help(char **tokens, char **envp)
{
	if (handle_redirections(tokens) == -1)
	{
		write(2, "redirection error\n", 18);
		free_tokens(tokens);
		exit(EXIT_FAILURE);
	}
	command_help2(tokens, envp);
	free_tokens(tokens);
	exit(127);
}

static void	handle_command(char **tokens, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		write(2, "fork error\n", 11);
		return ;
	}
	if (pid == 0)
		command_help(tokens, envp);
	waitpid(pid, &g_exit_status, 0);
	if (WIFEXITED(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
}

static void	handle_single_command(t_shell *shell, char *input)
{
	char	**tokens;

	tokens = ft_split(input, ' ');
	free(input);
	if (!tokens[0])
		return (free_tokens(tokens));
	if (ft_strchr(tokens[1], '"') || ft_strchr(tokens[1], '\''))
	{
		handle_bunnies(tokens, '\'', 0);
		handle_bunnies(tokens, '"', 1);
	}
	else
		are_we_rich(tokens);
	if (!handle_builtin(tokens, shell))
		handle_command(tokens, shell->envp);
	free_tokens(tokens);
}

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
			write(1, "\n\033[38;2;255;105;180mBye Bitch ;*\033[0m\n", 37);
			break ;
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
