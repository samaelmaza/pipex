/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sreffers <sreffers@student.42madrid.c>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:59:37 by sreffers          #+#    #+#             */
/*   Updated: 2025/11/18 16:59:07 by sreffers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error()
{
		ft_putstr(ERROR_MESSAGE, STDERR_FILENO);
		exit(EXIT_FAILURE);
}
void	run_command(int fd_in, int fd_out, char *cmd, char **env)
{
	char	**args;

	args = ft_split(cmd, ' ');
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
	{
		run_command(fd_in, pipefd[1], av[2], env);
		close(pipefd[1]);
	}
	pid2 = fork();
	if(pid2 < 0)
		error();
	if(pid2 == 0)
	{
		run_command(pipefd[0], fd_out, av[3], env);
		close(pipefd[0]);
	}
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}
