#include <unistd.h>

int	main(int ac, char **av)
{
	int i;
	int j;
	unsigned char array[256];

	if (ac == 3)
	{
		i = 0;
		while (av[1][i])
		{
			if (!array[(unsigned char)av[1][i]])
			{
				write(1, &av[1][i], 1);
				array[(unsigned char)av[1][i]] = 1;
			}
			i++;
		}
		while (av[2][j])
		{
			if (!array[(unsigned char)av[2][j]])
			{
				write(1, &av[2][j], 1);
				array[(unsigned char)av[2][j]] = 1;
			}
			j++;
		}
	}
	write(1, "\n", 1);
	return(0);
}