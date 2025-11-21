/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 12:36:22 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/21 13:21:27 by sreffers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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
	if (ft_strncmp(line, limiter, len) == 0 && line[len] == '\n')
		return (1);
	return (0);
}

int	handle_here_doc(char *limiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) < 0)
		error(ERROR_PIPE);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line || is_limiter(line, limiter))
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	close (pipefd[1]);
	return (pipefd[0]);
}
