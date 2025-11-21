/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:59:37 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/21 13:20:41 by sreffers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	main(int ac, char **av, char **env)
{
	int		n_cmd;
	t_cmd	*cmds;
	t_pipex	px;

	px.is_here_doc = check_here_doc(ac, av);
	n_cmd = ac - 3;
	if (px.is_here_doc)
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
