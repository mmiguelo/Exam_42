#include <unistd.h>

int	main(int ac, char **av)
{
	int i = 0;
	int j;
	unsigned char array[256];

	if (ac == 3)
	{
		while (av[1][i])
		{
			j = 0;
			while (av[2][j])
			{
				if (av[1][i] == av[2][j])
				{
					if (!array[(unsigned char)av[1][i]])
					{
						write(1, &av[1][i], 1);
						array[(unsigned char)av[1][i]] = 1;
					}
					break;
				}
				j++;
			}
			i++;
		}
	}
	write(1, "\n", 2);
	return (0);
}
