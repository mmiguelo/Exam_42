#include <stdlib.h>
#include <unistd.h>

int main(int ac, char **av)
{
	if (ac > 1)
	{
		int begin;
		int end;
		int i = 0;
		int flag = 0;

		while(av[1][i] && (av[1][i] == ' ' || av[1][i] == '\t'))
			i++;
		begin = i;
		while(av[1][i] && av[1][i] != ' ' && av[1][i] != '\t')
			i++;
		end = i - 1;
		while(av[1][i] && (av[1][i] == ' ' || av[1][i] == '\t'))
			i++;
		if (av[1][i])
		{
			while(av[1][i])
			{
				if (av[1][i] == ' ' || av[1][i] == '\t')
				{
					while (av[1][i] == ' ' || av[1][i] == '\t')
					{
						i++;
						flag++;
					}
				}
				else
				{
					if (flag)
						write(1, " ", 1);
					write(1, &av[1][i], 1);
					i++;
					flag = 0;
				}
			}
			write(1, " ", 1);
		}
		while(begin <= end)
		{
			write(1, &av[1][begin], 1);
			begin++;
		}
	}
	write(1, "\n", 1);
}
