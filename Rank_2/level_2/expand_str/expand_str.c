#include <unistd.h>

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		int i = 0;
		while (av[1][i] == 9 || av[1][i] == 32)
			i++;
		while (av[1][i])
		{
			if (av[1][i] != ' ' && av[1][i] != 9)
				write(1, &av[1][i], 1);
			if (av[1][i] == ' ' || av[1][i] == 9)
			{
				while ((av[1][i] == ' ' || av[1][i] == 9) && av[1][i + 1] == ' ' || av[1][i + 1] == 9)
				 	i++;
				write(1, &av[1][i], 1);
				write(1, "  ", 2);
			}
			i++;
		}
	}
	write(1, "\n", 1);
}
