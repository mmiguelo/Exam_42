#include <unistd.h>
#include <stdlib.h>

int ft_atoi(char *str)
{
	int i = 0;
	int result = 0;

	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		i++;
	}
	return (result);
}
int	ft_isprime(int nbr)
{
	int i = 2;

	if (nbr <= 1)
		return (0);
	while (i * i <= nbr)
	{
		if (nbr % i != 0)
			i++;
		else
			return (0);
	}
	return (1);
}

void	ft_putnbr(int nbr)
{
	char	c;

	if (nbr >= 10)
		ft_putnbr(nbr / 10);
	c = nbr % 10 + '0';
	write(1, &c, 1);
}
int	main(int ac, char **av)
{
	if (ac ==2)
	{
		int result = 0;
		int i = 2;
		int nbr = ft_atoi(av[1]);
		while (i <= nbr)
		{
			if (ft_isprime(i))
				result += i;
			i++;
		}
		ft_putnbr(result);
	}
	if (ac != 2)
		ft_putnbr(0);
	write(1, "\n", 1);
	return (0);
}
