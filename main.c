/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:59:37 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/18 21:15:41 by sreffers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error()
{
		ft_putstr(ERROR_MESSAGE, STDERR_FILENO);
		exit(EXIT_FAILURE);
}
char	*find_path(char **env)
{
	int		i;
	char	*path;

	i = 0;
	while(env[i])
	{
		if(ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = env[i] + 5;
			break;
		}
		i++;
	}
	return (path);
}
char	*find_correct_path(char **path, char *cmd)
{
	int		i;
	char	*correct_path;

	i = 0;
	while()
}
void	run_command(int fd_in, int fd_out, char *cmd, char **env)
{
	char	**args;
	char	**path;
	char	*correct_path;

	args = ft_split(cmd, ' ');
	path = ft_split(find_path(env), ':');
	correct_path = find_correct_path(path, args[0]);
	if(!args)
		error();
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	if(fd_in != STDIN_FILENO)
		close(fd_in);
	if(fd_out != STDOUT_FILENO)
		close(fd_out);

	execve(args[0], args, env);
	error();
}
int	main(int ac, char **av, char **env)
{
	int	pipefd[2];
	int	fd_in;
	int	fd_out;
	pid_t pid1;
	pid_t pid2;

	if(ac != 5)
		error();
	fd_in = open(av[1], O_RDONLY);
	fd_out = open(av[4], O_WRONLY | O_CREAT | O_TRUNC | FILE_PERM);
	if(pipe(pipefd) < 0)
		error();
	pid1 = fork();
	if(pid1 < 0)
		error();
	if(pid1 == 0)
		run_command(fd_in, pipefd[1], av[2], env);
	pid2 = fork();
	if(pid2 < 0)
		error();
	if(pid2 == 0)
		run_command(pipefd[0], fd_out, av[3], env);

	close(pipefd[1]);
	close(pipefd[0]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}
