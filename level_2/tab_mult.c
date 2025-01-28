#include <unistd.h>

int	ft_atoi(char *str)
{
	int i = 0;
	int sign = 1;
	int result = 0;

	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

void	put_nbr(int n)
{
	char c;

	if (n == -2147483648)
		{
			write(1, "-2147483648", 11);
			return;
		}
	if (n < 0)
	{
		write(1, "-", 1);
		n = -n;
	}
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
