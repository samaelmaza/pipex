/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 12:04:13 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/18 17:00:57 by sreffers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>

# define ERROR_MESSAGE "Error\n"
# define FILE_PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void	ft_putstr(char *s, int fd);
char	**ft_split(char const *s, char c);

#endif
