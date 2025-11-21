/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 12:04:13 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/21 15:54:04 by sreffers         ###   ########.fr       */
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
# include "../gnl/get_next_line.h"

# define ERROR_MESSAGE "Error\n"
# define ERROR_MALLOC "Error with malloc\n"
# define ERROR_PATH "Error with the access of the path\n"
# define ERROR_EXEC "Error with the execution of execve\n"
# define ERROR_PIPE "Error with the creation of the pipe\n"
# define ERROR_FORK "Error with the execution of the fork\n"
# define ERROR_WAIT "Error with the execution of waitpid\n"
# define ERROR_ARG "Please enter enter the right amount of arguments\n"
# define ERROR_FILE "Error with the opening of the file\n"

typedef struct s_cmd
{
	int		fd_in;
	int		fd_out;
	char	*cmd;
	char	**env;
}	t_cmd;

typedef struct s_pipex
{
	int	*pids;
	int	**pipes;
	int	fd_in;
	int	fd_out;
	int	n_cmds;
	int	is_here_doc;
}	t_pipex;

void	ft_putstr(char *s, int fd);
char	**ft_split(char const *s, char c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strlen(char *str);
char	*find_path(char **env);
void	build_correct_path(char *path, char *cmd, char *correct_path);
char	*find_correct_path(char **path, char *cmd);
int		check_here_doc(int ac, char **av);
int		is_limiter(char *line, char *limiter);
int		handle_here_doc(char *limiter);
void	init_pipex(t_pipex *px, int n_cmds);
void	prepare_commands(t_cmd *cmds, t_pipex *px, char **av, char **env);
void	run_command(char *cmd, char **env);
pid_t	fork_command(t_cmd *cmd, int *pipe_prev, int *pipe_next, t_pipex *px);
void	launch_all(t_pipex *px, t_cmd *cmd);
int		wait_all(t_pipex *px);
void	open_files(t_pipex *px, char **av, int ac, int is_here_doc);
void	error(char *s);
void	free_split(char **args);
void	close_pipes(t_pipex *px, t_cmd *cmd);
void	close_all(t_pipex *px);

#endif
