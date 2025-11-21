/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 12:42:53 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/21 12:45:36 by sreffers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	run_command(char *cmd, char **env)
{
	char	**args;
	char	**path;
	char	*correct_path;

	args = ft_split(cmd, ' ');
	path = ft_split(find_path(env), ':');
	if (!path || !args)
		error(ERROR_MALLOC);
	correct_path = find_correct_path(path, args[0]);
	if (!correct_path)
	{
		free_split(args);
		free_split(path);
		error(ERROR_PATH);
	}
	execve(correct_path, args, env);
	free_split(args);
	free_split(path);
	free(correct_path);
	error(ERROR_EXEC);
}

pid_t	fork_command(t_cmd *cmd, int *pipe_prev, int *pipe_next, t_pipex *px)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		error(ERROR_FORK);
	if (pid == 0)
	{
		if (pipe_prev)
			dup2(pipe_prev[0], STDIN_FILENO);
		else
			dup2(cmd->fd_in, STDIN_FILENO);
		if (pipe_next)
			dup2(pipe_next[1], STDOUT_FILENO);
		else
			dup2(cmd->fd_out, STDOUT_FILENO);
		close_pipes(px, cmd);
		run_command(cmd->cmd, cmd->env);
	}
	return (pid);
}

void	launch_all(t_pipex *px, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < px->n_cmds)
	{
		if (i == 0)
			px->pids[i] = fork_command(&cmd[i], NULL, px->pipes[i], px);
		else if (i == px->n_cmds - 1)
			px->pids[i] = fork_command(&cmd[i], px->pipes[i - 1], NULL, px);
		else
			px->pids[i] = fork_command(
					&cmd[i],
					px->pipes[i - 1],
					px->pipes[i],
					px);
		i++;
	}
}

void	wait_all(t_pipex *px)
{
	int	i;
	int	status;

	i = 0;
	while (i < px->n_cmds)
	{
		if (waitpid(px->pids[i], &status, 0) < 0)
			error(ERROR_WAIT);
		i++;
	}
}
