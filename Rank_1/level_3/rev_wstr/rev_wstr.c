#include <unistd.h>
/* #include <stdlib.h> */
#include <stdio.h>

int main(int ac, char **av)
{
	if (ac == 2)
	{
		int end;
		int start;
		int flag;
		int i = 0;
		while(av[1][i])
			i++;
		while (i >= 0)
		{
			while(av[1][i] == ' ' || av[1][i] == '\0' || av[1][i] == '\t')
				i--;
			end = i;
			while (av[1][i] && av[1][i] != ' ' && av[1][i] != '\t')
				i--;
			start = i + 1;
			flag = start;
			while (start <= end)
				write(1, &av[1][start++], 1);
			if (flag)
				write(1, " ", 1);
		}
		
	}
	write(1, "\n", 1);
	return (0);
}