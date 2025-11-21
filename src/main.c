/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:59:37 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/21 01:43:17 by sreffers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	error(char *s)
{
	ft_putstr(s, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	free_split(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

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

void	init_pipex(t_pipex *px, int n_cmds)
{
	int	i;

	px->n_cmds = n_cmds;
	px->pids = malloc(sizeof(pid_t) * n_cmds);
	px->pipes = malloc(sizeof(int *) * (n_cmds - 1));
	if (!px->pids || !px->pipes)
		error(ERROR_MALLOC);
	i = 0;
	while (i < n_cmds - 1)
	{
		px->pipes[i] = malloc(sizeof(int) * 2);
		if (!px->pipes[i])
			error(ERROR_MALLOC);
		if (pipe(px->pipes[i]) < 0)
			error(ERROR_PIPE);
		i++;
	}
}

void	prepare_commands(t_cmd *cmds, t_pipex *px, char **av, char **env)
{
	int	i;
	int	offset;

	i = 0;
	if(px->is_here_doc)
		offset = 3;
	else
		offset = 2;
	while (i < px->n_cmds)
	{
		cmds[i].fd_in = px->fd_in;
		cmds[i].fd_out = px->fd_out;
		cmds[i].env = env;
		cmds[i].cmd = av[offset + i];
		i++;
	}
}

void	close_pipes(t_pipex *px, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < px->n_cmds - 1)
	{
		close(px->pipes[i][0]);
		close(px->pipes[i][1]);
		i++;
	}
	close(cmd->fd_in);
	close(cmd->fd_out);
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

void	close_all(t_pipex *px)
{
	int	i;

	i = 0;
	while (i < px->n_cmds - 1)
	{
		close(px->pipes[i][0]);
		close(px->pipes[i][1]);
		free(px->pipes[i]);
		i++;
	}
	free(px->pipes);
	close(px->fd_in);
	close(px->fd_out);
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

int	check_here_doc(int ac, char **av)
{
	if (ac >= 2 && ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		if (ac < 6)
			error(ERROR_ARG);
		return (1);
	}
	if (ac < 5)
		error(ERROR_ARG);
	return (0);
}

int	is_limiter(char *line, char *limiter)
{
	int	len;

	len = ft_strlen(limiter);
	if(ft_strncmp(line, limiter, len) == 0 && line[len] == '\n')
		return (1);
	return (0);
}
int	handle_here_doc(char *limiter)
{
	int		pipefd[2];
	char	*line;

	if(pipe(pipefd) < 0)
		error(ERROR_PIPE);
	while(1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if(!line || is_limiter(line, limiter))
		{
			free(line);
			break;
		}
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

void	open_files(t_pipex *px, char **av, int ac, int is_here_doc)
{
	if(is_here_doc)
	{
		px->fd_in = handle_here_doc(av[2]);
		px->fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		px->fd_in = open(av[1], O_RDONLY);
		if(px->fd_in < 0)
			error(ERROR_FILE);
		px->fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (px->fd_out < 0)
			error(ERROR_FILE);
	}
}

int	main(int ac, char **av, char **env)
{
	int		n_cmd;
	t_cmd	*cmds;
	t_pipex	px;

	px.is_here_doc = check_here_doc(ac, av);
	n_cmd = ac - 3;
	if(px.is_here_doc)
		n_cmd = ac - 4;
	open_files(&px, av, ac, px.is_here_doc);
	init_pipex(&px, n_cmd);
	cmds = malloc(sizeof(t_cmd) * n_cmd);
	if (!cmds)
		error(ERROR_MALLOC);
	prepare_commands(cmds, &px, av, env);
	launch_all(&px, cmds);
	close_all(&px);
	wait_all(&px);
	free(cmds);
	free(px.pids);
}
