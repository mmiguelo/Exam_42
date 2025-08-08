#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	int pid = 0;

	if(!file || !argv)
		return -1;
	if(pipe(fd) == -1)
		return -1;
	if (type == 'r')
	{
		pid = fork();
		if(pid == -1)
			return (close(fd[0]), close(fd[1]), -1);
		else if (pid == 0)
		{
			if (dup2(fd[1], 1) == -1)
			{
				close (fd[0]);
				close (fd[1]);
				exit(1);
			}
			close(fd[0]);
			close(fd[1]);
			execvp(file, argv);
			exit(1);
		}
		close(fd[1]);
		return (fd[0]);
	}
	else if(type == 'w')
	{
		pid = fork();
		if(pid == -1)
			return (close(fd[0]), close(fd[1]), -1);
		if (pid == 0)
		{
			if (dup2(fd[0], 0) == -1)
			{
				close(fd[0]);
				close(fd[1]);
				exit(1);
			}
			close(fd[0]);
			close(fd[1]);
			execvp(file, argv);
			exit(1);
		}
		close(fd[0]);
		return (fd[1]);
	}
	return (close(fd[0]), close(fd[1]), -1);
}