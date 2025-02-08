#include <unistd.h>

int	ft_atoi(char *str)
{
	int i = 0;
	int result = 0;

	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

void	put_nbr(int n)
{
	char c;

	if (n > 9)
		put_nbr(n / 10);
	c = n % 10 + '0';
	write(1, &c, 1);
}

int main(int ac, char **av)
{
	if (ac == 2)
	{
		int i = 1;
		int nb = ft_atoi(av[1]);

		while (i <= 9)
		{
			put_nbr(i);
			write(1, " x ", 3);
			put_nbr(nb);
			write(1, " = ", 3);
			put_nbr(nb * i);
			write(1, "\n", 1);
			i++;
		}
	}
	else
		write(1, "\n", 1);
	return (0);
}
