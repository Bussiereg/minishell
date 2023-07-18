/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbussier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 19:06:56 by gbussier          #+#    #+#             */
/*   Updated: 2023/07/13 18:59:54 by gbussier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(char **cmd, t_minishell *mini)
{
	if (!ft_strncmp(cmd[0], "pwd", max_length("pwd", cmd[0])))
		return (pwd_builtin(mini, cmd));
	else if (!ft_strncmp(cmd[0], "env", max_length("env", cmd[0])))
		return (env_builtin(mini));
	else if (!ft_strncmp(cmd[0], "cd", max_length("cd", cmd[0])))
		return (cd_builtin(cmd, mini));
	else if (!ft_strncmp(cmd[0], "export", max_length("export", cmd[0])))
		return (export_builtin(cmd, mini));
	else if (!ft_strncmp(cmd[0], "unset", max_length("unset", cmd[0])))
		return (unset_builtin(cmd, mini));
	else if (!ft_strncmp(cmd[0], "echo", max_length("echo", cmd[0])))
		return (echo_builtin(cmd));
	else if (!ft_strncmp(cmd[0], "exit", max_length("exit", cmd[0])))
		return (exit_builtin(cmd, mini));
	return (EXIT_FAILURE);
}

int	exec(char **cmd, char **envp, t_minishell *mini)
{
	char	*path;

	if (is_builtin(cmd[0]) == EXIT_SUCCESS)
		return (execute_builtin(cmd, mini));
	else
	{
		path = find_executable(cmd, mini);
		if (!path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd[0], 2);
			ft_putendl_fd(": command not found", 2);
			ft_free(path, NULL, NULL, NULL);
			return (127);
		}
		else if (execve(path, cmd, envp) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd[0], 2);
			ft_putendl_fd(": command not found", 2);
			ft_free(path, NULL, NULL, NULL);
			return (127);
		}
	}
	return (EXIT_FAILURE);
}

void	handler(int n)
{
	ft_putendl_fd("Quit in the child", 2);
	(void)n;
}

int	execute_single_command(t_minishell *mini)
{
	pid_t	pid;
	int		exit_status;
	int		status;

	if (is_env_function(mini->cmd_table[0][0]) == EXIT_SUCCESS)
	{
		redirection_function_insert(*mini, mini->redir_start);
		exit_status = exec(mini->cmd_table[0], mini->envp, mini);
		return (exit_status);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			redirection_function(*mini, mini->redir_start);
			exit_status = exec(mini->cmd_table[0], mini->envp, mini);
			exit(exit_status);
		}
		else
		{
			waitpid(pid, &status, 0);
			exit_status = WEXITSTATUS(status);
			if (WTERMSIG(status) == SIGINT)
				exit_status = 130;
			if (WTERMSIG(status) == SIGQUIT)
				exit_status = 131;
		}
	}
	return (exit_status);
}

int	execute_several_commands(t_minishell *mini, int index)
{
	int		**fd;
	int		exit_status;
	int		status;
	int		exit_status2;
	pid_t	pid;

	pid = 0;
	exit_status = 0;
	exit_status2 = 0;
	fd = create_pipe(mini);
	while (index < mini->nb_cmd)
	{
		if ((is_env_function(mini->cmd_table[index][0]) == 0)
			&& (index == mini->nb_cmd - 1))
		{
			outfile_insert(mini->redir_end);
			close(fd[index - 1][0]);
			exit_status2 = exec(mini->cmd_table[index], mini->envp, mini);
		}
		else if (index == mini->nb_cmd - 1)
			pid = create_process_fd(mini->cmd_table[index], mini, index, fd);
		else
			create_process_fd(mini->cmd_table[index], mini, index, fd);
		index++;
	}
	if (pid != 0)
	{
		waitpid(pid, &status, 0);
		exit_status = WEXITSTATUS(status);
		if (WTERMSIG(status) == SIGINT)
			exit_status = 130;
		if (WTERMSIG(status) == SIGQUIT)
			exit_status = 131;
	}
	while (wait(NULL) != -1)
		;
	free_pipe(fd);
	if ((is_env_function(mini->cmd_table[mini->nb_cmd - 1][0]) == EXIT_SUCCESS))
		return (exit_status2);
	return (exit_status);
}

int	executor(t_minishell *mini)
{
	pid_t pid;

	if (mini->error == 1)
		return (0);
	signal_command(mini);
	if (mini->nb_cmd == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			redirection_function(*mini, mini->redir_start);
			exit(EXIT_SUCCESS);
		}
		else
			waitpid(pid, NULL, 0);
		return(1);
	}
	if (mini->nb_cmd == 1)
		mini->exit_status = execute_single_command(mini);
	else
		mini->exit_status = execute_several_commands(mini, 0);
	return (1);
}
