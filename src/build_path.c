/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 17:37:03 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/21 01:43:39 by sreffers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*find_path(char **env)
{
	int		i;
	char	*path;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = env[i] + 5;
			break ;
		}
		i++;
	}
	return (path);
}

void	build_correct_path(char *path, char *cmd, char *correct_path)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (path[i])
		correct_path[j++] = path[i++];
	correct_path[j++] = '/';
	i = 0;
	while (cmd[i])
		correct_path[j++] = cmd[i++];
	correct_path[j] = '\0';
}

char	*find_correct_path(char **path, char *cmd)
{
	int		i;
	char	*correct_path;

	i = 0;
	while (path[i])
	{
		correct_path = malloc(ft_strlen(path[i]) + 1 + ft_strlen(cmd) + 1);
		if (!correct_path)
			return (NULL);
		build_correct_path(path[i], cmd, correct_path);
		if (access(correct_path, X_OK) == 0)
			return (correct_path);
		free(correct_path);
		i++;
	}
	return (NULL);
}
