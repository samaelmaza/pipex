/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 12:39:35 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/21 13:21:49 by sreffers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	open_files(t_pipex *px, char **av, int ac, int is_here_doc)
{
	if (is_here_doc)
	{
		px->fd_in = handle_here_doc(av[2]);
		px->fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		px->fd_in = open(av[1], O_RDONLY);
		if (px->fd_in < 0)
			error(ERROR_FILE);
		px->fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (px->fd_out < 0)
			error(ERROR_FILE);
	}
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
	if (px->is_here_doc)
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
