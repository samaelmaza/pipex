/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_and_close.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 12:47:25 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/21 13:22:15 by sreffers         ###   ########.fr       */
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
